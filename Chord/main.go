package main

import (
	"log"
	"net"
	"net/http"
	"net/rpc"
	"bufio"
	"os"
	"strings"
	"strconv"
	"math/big"
	"crypto/sha1"
	"time"
)

//---------------// RANDOM STUFF //---------------//

func hashString(elt string) *big.Int {
    hasher := sha1.New()
    hasher.Write([]byte(elt))
    return new(big.Int).SetBytes(hasher.Sum(nil))
}

func between(start, elt, end *big.Int, inclusive bool) bool {
    if end.Cmp(start) > 0 {
        return (start.Cmp(elt) < 0 && elt.Cmp(end) < 0) || (inclusive && elt.Cmp(end) == 0)
    } else {
        return start.Cmp(elt) < 0 || elt.Cmp(end) < 0 || (inclusive && elt.Cmp(end) == 0)
    }
}

const keySize = sha1.Size * 8
var two = big.NewInt(2)
var hashMod = new(big.Int).Exp(big.NewInt(2), big.NewInt(keySize), nil)

func jump(address string, fingerentry int) *big.Int {
    n := hashString(address)
    fingerentryminus1 := big.NewInt(int64(fingerentry) - 1)
    jump := new(big.Int).Exp(two, fingerentryminus1, nil)
    sum := new(big.Int).Add(n, jump)

    return new(big.Int).Mod(sum, hashMod)
}

func find(id *big.Int, start string) string{
	next_node := start
	for i := 0; i < maxSteps; i++ {
		var args = Packet{Hash: id}
		var response Packet
		if err := call(next_node, "Node.FindSuccessor", &args, &response); err != nil {
			log.Printf("Error: call to FindSuccessor in find: %v", err)
		} else {
			i += 1

			//if found, return the successor to id
			if response.Bool == true {
				next_node = response.String
				return next_node
			} else {
				next_node = response.String
			}
		}
	}
	print("\nError: call to find: id was not found within maxSteps\n\n")
	return ""
}

var big_map = map[string]string {
	"mine": "yours", 
	"jeff": "jiff",
	"you": "me", 
	"me": "you", 
	"yesterday": "tomorrow", 
	"sunday": "monday", 
	"monday": "tuesday", 
	"tuesday": "wednesday",
	"wednesday": "thursday",
	"thursday": "friday",
	"friday": "saturday",
	"saturday": "sunday",
	"chicken": "wing",
	"BFG": "cucumber",
	"cause": "effect",
	"meow": "woof",
	"wool": "cotton",
	"no": "yes",
	"yes": "no",
	"black": "blue",
	"blue": "yellow",
	"green": "red",
	"purple": "teal",
	"back": "forth",
	"key": "value",
	"snore": "lax",
	"cough": "drop",
	"drop": "top",
	"one": "stop",
	"not": "flop",
	"seven": "eleven",
	"fish": "wish",
	"sword": "dagger",
	"sheild": "weild",
	"happy": "sad",
	"bed": "time",
	"sun": "moon"}

//defaults
const (
	defaultHost string = "localhost"
	defaultPort string = "3410"
	successorLength int = 3
	maxSteps int = 32
)

//getting network address
func getLocalAddress() string {
    conn, err := net.Dial("udp", "8.8.8.8:80")
    if err != nil {
        log.Fatal(err)
    }
    defer conn.Close()

    localAddr := conn.LocalAddr().(*net.UDPAddr)

    return localAddr.IP.String()
}

//---------------// SERVER //---------------//

func server(address string, port string, join_address string, shutdown chan int) {
	//make the new node
	node := new(Node)
	
	//Address
	node.Address = address + ":" + port

	//Successors
	node.Successors = make([]string, successorLength)
	for i := 0; i < successorLength; i++ {
		node.Successors[i] = node.Address
	}
	if(join_address != "blank_address") {
		node.Successors[0] = join_address
	}

	//Predecessor
	node.Predecessor = node.Address

	//FingerTable
	node.FingerTable = make([]string, 160)
	for i := 0; i < 160; i++ {
		node.FingerTable[i] = ""
	}

	//Bucket
	node.Bucket = make(map[string]string)

	var GetAll_args = Packet{String: node.Address}
	var GetAll_response Packet

	if node.Successors[0] != node.Address{
		if err := call(node.Successors[0], "Node.GetAll", &GetAll_args, &GetAll_response); err != nil {
			log.Printf("Error: could not get data items from successor")
		}

		node.Bucket = GetAll_response.Map
	}

	//register and listen
	rpc.Register(node)
	rpc.HandleHTTP()
	listener, e := net.Listen("tcp", ":" + port)
	if e != nil {
		log.Fatal("listen error: ", e)
	}
	
	//run background routines. ie Stablilize, checkpredecessor, and fixfingers
	go back_routines(node)

	//wait for quit
	go func() {
		<-shutdown

		var args = Packet{Map: node.Bucket}
		var response Packet
		if err := call(node.Successors[0], "Node.PutAll", &args, &response); err != nil {
			log.Printf("Failed to hand off bucket to successor: %v", err)
		}
		os.Exit(0)
	}()

	if err := http.Serve(listener, nil); err != nil {
		log.Fatalf("http.Serve: %v", err)
	}
}

func back_routines(node *Node) {
	var args []string
	var response []string
	next := 0 //initialize next. used for fixfingers
	for true{
		//Stabilize
		//checkpredecessor was implemented within notify(within stabilize)
		node.Stabilize(&args, &response)
		time.Sleep(500 * time.Millisecond)

		//fixfingers
		var fix_args = Packet{Int: next}
		var fix_response Packet
		node.FixFingers(&fix_args, &fix_response)
		next = fix_response.Int
		time.Sleep(500 * time.Millisecond)

	}
}

//used to rpc call on server from client
func call(address string, method string, request interface{}, response interface{}) error {
	client, err := rpc.DialHTTP("tcp", address)
	if err != nil {
		//log.Printf("rpc.Dial: %v", err)
		return err
	}
	defer client.Close()

	if err = client.Call(method, request, response); err != nil {
		log.Printf("!!!Client call to the method %s: %v!!!", method, err)
		return err
	}

	return nil
}

func helpStartup() {
	commands := []string{"help", "port <n>", "create", "join <address>", "quit"}
	print("\n---AVAILABLE COMMANDS---\n")
	for _, command := range commands {
		print(command + "\n")
	}
}

func help() {
	commands := []string{"help", "put <key> <value>", "putrandom <n>", "get <key>", "delete <key>", "dump", "quit"}
	print("\n---AVAILABLE COMMANDS---\n")
	for _, command := range commands {
		print(command + "\n")
	}
}

func helpDescriptions(n int) {
	switch n {
	case 1:
		print("\n---COMMAND DESCRIPTIONS---\n")
		print("command: help\n")
		print("-dislpays all available commands\n\n")
		print("command: port <n>\n")
		print("-set the port that this node should listen on\n\n")
		print("command: create\n")
		print("-create a new ring\n\n")
		print("command: join <address>\n")
		print("-join an existing ring, one of whose nodes is at the address specified\n\n")
		print("command: quit\n")
		print("-leave Chord\n\n")
	case 2:
		print("\n---COMMAND DESCRIPTIONS---\n")
		print("command: help\n")
		print("-dislpays all available commands\n\n")
		print("command: put <key> <value>\n")
		print("-insert the given key and value into the currently active ring\n\n")
		print("command: putrandom <n>\n")
		print("-randomly generate n keys (and accompanying values) and put each pair into the ring\n\n")
		print("command: get <key>\n")
		print("-find the given key in the currently active ring\n\n")
		print("command: delete <key>\n")
		print("-delete value at given key\n\n")
		print("command: dump\n")
		print("-display information about the current node\n\n")
		print("command: quit\n")
		print("-leave Chord\n\n")
	}
}

//---------------// Node //---------------//

type Node struct {
    Address     string //NodeAddress
    FingerTable []string //NodeAddress
    Predecessor string //NodeAddress
    Successors  []string //NodeAddress

    Bucket map[string]string
}

//this packet helps with rpc calls
//used to send in and out whatever kind of data required
//functions are documented with usage
type Packet struct {
	Stringlist	[]string
	Intlist		[]int
	String		string
	Int		int
	Bool		bool
	Map		map[string]string
	Hash		*big.Int
}

func (n *Node) Get(args *[]string, response *[]string) error {
	//args- [key]
	//response- [value]
	key := (*args)[0]
	value, ok := n.Bucket[key]
	if ok {
		var val = []string{value}
		*response = make([]string, 1)
		copy(*response, val)
	} else {
		var val = []string{"key not found"}
		*response = make([]string, 1)
		copy(*response, val)
	}
	return nil
}

func (n *Node) GetPredecessor(args *[]string, pkt *Packet) error {
	var args1 []string
	var response1 []string
	if err := call(n.Predecessor, "Node.Ping", &args1, &response1); err != nil {
		print("Predecessor dropped out\n\n")
		n.Predecessor = ""
	}
	pkt.String = n.Predecessor
	pkt.Stringlist = n.Successors
	return nil
}

func (n *Node) Notify(args *[]string, response *[]string) error {
	//args- [address]
	//response- empty

	if n.Predecessor == "" {
		n.Predecessor = (*args)[0]
	}
	caller := hashString((*args)[0])
	me := hashString(n.Address)
	predecessor := hashString(n.Predecessor)
	if between(predecessor, caller, me, false) {
		n.Predecessor = (*args)[0]
	}
	return nil
}

func (n *Node) Stabilize(args *[]string, response *[]string) error {
	//args- empty
	//response- empty

	var args1 []string
	var pkt1 Packet
	if err := call(n.Successors[0], "Node.GetPredecessor", &args1, &pkt1); err != nil {

		//if immediate successor doesnt respond, remove from successor list
		print("\n" + n.Successors[0] + " dropped out\n\n")
		new_successors := make([]string, successorLength)
		for i := 0 ; i < len(n.Successors) - 1; i++ {
			new_successors[i] = n.Successors[i + 1]
		}
		n.Successors = new_successors

	} else {
		//check if successors predecessor is between me and my successor
		//update successor list based on result

		//my successors predecessor
		x := pkt1.String

		if x != "" {
			successorlist := pkt1.Stringlist

			//prepare hashed locations for between call
			xhash := hashString(x)
			me := hashString(n.Address)
			successor := hashString(n.Successors[0])

			if between(me, xhash, successor, false) {

				//between
				newlist := []string{x, n.Successors[0], successorlist[0]}
				n.Successors = newlist
			} else {

				//not between
				newlist := []string{n.Successors[0], successorlist[0], successorlist[1]}
				n.Successors = newlist
			}
		}	
	}
	var args2 = []string{n.Address}
	var response2 []string
	if err := call(n.Successors[0], "Node.Notify", &args2, &response2); err != nil {
		print("\n" + n.Successors[0] + " isnt responding. Adjusting successor list\n\n")
		new_successors := make([]string, successorLength)
		for i := 0 ; i < len(n.Successors) - 1; i++ {
			new_successors[i] = n.Successors[i + 1]
		}
		n.Successors = new_successors
	}
	return nil
}

func (n *Node) FindSuccessor(args *Packet, response *Packet) error {
	//args- args.Hash = hashed id(type *big.Int)
	//response- response.Bool = found, response.String = successor

	target := args.Hash

	//prepare hashes for between call
	me := hashString(n.Address)
	successor_hash := hashString(n.Successors[0])

	if between(me, target, successor_hash, true) {
		response.Bool = true
		response.String = n.Successors[0]
	} else {
		response.Bool = false
		var closest_args = Packet{Hash: target}
		var closest_response Packet
		n.ClosestPreceedingNode(&closest_args, &closest_response)
		response.String = closest_response.String
	}

	return nil		
}

func (n *Node) ClosestPreceedingNode(args *Packet, response *Packet) error {
	//args- args.Hash = target
	//response- response.String = address(closest node)
	
	target := args.Hash
	me := hashString(n.Address)
	found := false

	for i := 159; i >= 0; i-- {
		if n.FingerTable[i] == "" {
			continue
		}
		entry := hashString(n.FingerTable[i])
		if between(me, entry, target, true) {
			response.String = n.FingerTable[i]
			found = true
		}	
	}
	if !found {
		response.String = n.Successors[0]
	}
	return nil
}

func (n *Node) PutAll(args *Packet, response *Packet) error {
	//args- args.Map = (key: value) pairs to add to bucket
	//response- empty

	for key, value := range args.Map {
		var A = []string{key, value}
		var R []string
		if err := n.Put(&A, &R); err != nil {
			return err
		}
	}
	return nil
}

func (n *Node) GetAll(args *Packet, response *Packet) error {
	//args- args.String = new address (predecessor that just entered the ring)
	//response- packet.Map = map of items the new node will take over

	me := hashString(n.Address)
	new_node := hashString(args.String)
	response.Map = make(map[string]string)

	for key, value := range n.Bucket {
		key_hash := hashString(key)

		if between(key_hash, new_node, me, true) {
			response.Map[key] = value
			delete(n.Bucket, key)
		}
	}
	return nil
}

func (n *Node) FixFingers(fix_args *Packet, fix_response *Packet) error {
	next := fix_args.Int
	if next >= 160 {
		next = 0
	}

	big_int := jump(n.Address, next + 1) //one based
	
	successor := find(big_int, n.Address)
	n.FingerTable[next] = successor

	next += 1
	for i := next; i < 160; i++ {
		big_int := jump(n.Address, next + 1)
		me := hashString(n.Address)
		entry := hashString(successor)
		if between(me, big_int, entry, true) {
			n.FingerTable[next] = n.FingerTable[next-1]
			next += 1
		} else {
			break
		}
	}
	fix_response.Int = next
	return nil
}

func (n *Node) Put(args *[]string, response *[]string) error {
	//args- [key, value]
	//response- empty

	key := (*args)[0]
	value := (*args)[1]
	n.Bucket[key] = value
	return nil
}

func (n *Node) Delete(args *[]string, response *[]string) error {
	//args- [key]
	//response- empty

	key := (*args)[0]
	delete(n.Bucket, key)
	return nil
}

func (n *Node) Ping(args *[]string, response *[]string) error {
	var pong = []string{"PONG\n"}
	*response = make([]string, 1)
	copy(*response, pong)
	//print("ping requested\n")
	return nil
}

func (n *Node) Dump(args *[]string, response *Node) error {
	//copies the node and sends it back
	_ = args
	*response = *n
	return nil
}

//---------------// MAIN //---------------//

func main() {
	print("Welcome to Chord\n\n")
	helpStartup()
	print("\n")

	addy := defaultHost
	port := defaultPort
	S := false
	shutdown := make(chan int)

	scanner := bufio.NewScanner(os.Stdin)
	for scanner.Scan() {
		line := scanner.Text()
		line = strings.TrimSpace(line)

		parts := strings.SplitN(line, " ", 3)
		if len(parts) > 1 {
			parts[1] = strings.TrimSpace(parts[1])
		}

		if len(parts) == 0 {
			continue
		}

		//main client loop
		switch parts[0] {
		case "help":
			if S {
				help()
				helpDescriptions(2)
			} else {
				helpStartup()
				helpDescriptions(1)
			}
		case "quit":
			print("\nquiting...\n\n")
			shutdown <- 0
		case "port":
			if(!S) {
				if len(parts) < 2 {
					print("\n!You must include a port number!\n\n")
				} else {
					port = parts[1]
					print("Changed port to: " + parts[1] + "\n\n")
				}
			} else {
				log.Printf("You cannot change Port after joining a ring")
			}
		case "create":
			if(!S) {
				S = true
				blank_address := "blank_address"
				print("Starting up the Server... \n\n")
				print("Successfully created a new Chord ring\n\n")

				go server(addy, port, blank_address, shutdown)
			} else {
				log.Printf("You cannot create a new ring while you are already in one")
			}

		case "join":
			if(S) {
				print("You are already participating in an active Chord ring.\n\n")
			} else {
				if(len(parts) != 2) {
					print("Incorrect amount of arguments\n")
					print("Usage: join <address>")
				} else {
					//ping the address to make sure it is an active node
					var args []string
					var response []string
					join_address := parts[1]
					if err := call(join_address, "Node.Ping", &args, &response); err != nil {
						print("Host: " + join_address + " is not active\n\n")
					} else{
						print("Starting up the server...\n")
						S = true
						
						id := hashString(addy + ":" + port)
						found_successor := find(id, join_address)
						if found_successor == "" {
							log.Printf("Error finding true Successor")
						} else {
							join_address = found_successor
						}
						
						print("Joining on " + join_address + "...\n\n")
						go server(addy, port, join_address, shutdown)
						print("Join success\n\n")
					}
				}
			}
		case "ping":
			if(len(parts) != 2) {
				print("Incorrect amount of arguments")
			} else {
				contact_address := parts[1]
				var response []string
				var args []string
				if err := call(contact_address, "Node.Ping", &args, &response); err != nil {
					log.Printf("Client call _Ping_: %v", err)
				} else {
					print(response[0])
				}
			}
		case "get":
			//check to make sure server is running
			if(!S) {
				print("You must be in a Chord ring to call this function.\n\n")
			} else{
				//check the function recieves needed parameters
				if(len(parts) != 2) {
					print("Incorrect amount of arguments.\n")
					print("Usage: get <key>\n\n")
				} else {
					key := parts[1]
					var response []string
					var args = []string{key}
					key_hash := hashString(key)

					//find the node that has the key
					found_address := find(key_hash, defaultHost + ":" + port)

					//call the funcion
					if err := call(found_address, "Node.Get", &args, &response); err != nil {
						log.Printf("Client call _Get_: %v", err)
					} else {
						print(response[0] + "\n\n")
					}
				}
			}
		case "put":
			if(!S) {
				print("You must be in a Chord ring to call this function.\n\n")
			} else {
				if(len(parts) != 3) {
					print("Incorrect amount of arguments.\n")
					print("Usage: put <key> <value>\n\n")
				} else {
					key := parts[1]
					value := parts[2]
					var response []string
					var args = []string{key, value}
					key_hash := hashString(key)

					found_address := find(key_hash, defaultHost + ":" + port)

					//call the function
					if err := call(found_address, "Node.Put", &args, &response); err != nil {
						log.Printf("Client call _Put_: %v", err)
					} else {
						print("Put successful.\n\n")
					}
				}
			}
		case "putrandom":
			if(!S) {
				print("You must be in a Chord ring to call this function\n\n")
			} else {
				if(len(parts) != 2) {
					print("Incorrect amount of arguments\n")
					print("Usage: putrandom <n>\n\n")
				} else {
					reps, err := strconv.Atoi(parts[1])
					if err != nil {
						print("ATOI error\n\n")
					} else {
						count := 0
						for key, value := range big_map {
							count += 1
							var args = []string{key, value}
							var response []string
							key_hash := hashString(key)
							if count >= reps {
								break
							} else {
								found_address := find(key_hash, defaultHost + ":" + port)

								if err := call(found_address, "Node.Put", &args, &response); err != nil {
									log.Printf("Client call _Put_: %v", err)
								}
							}
						}
						print("\nPutRandom successful\n\n")
					}
				}
			}
		case "delete":
			if(!S) {
				print("You must be in a Chord ring to call this function.\n\n")
			} else {
				if(len(parts) != 2) {
					print("Incorrect amount of arguments.\n")
					print("Usage: delete <key>\n\n")
				} else {
					key := parts[1]
					var response []string
					var args = []string{key}
					key_hash := hashString(key)

					found_address := find(key_hash, defaultHost + ":" + port)

					if err := call(found_address, "Node.Delete", &args, &response); err != nil {
						log.Printf("Client call _Delete_: %v", err)
					} else {
						print("Delete complete.\n\n")
					}
				}
			}
		case "dump":
			if(!S) {
				print("You must be in a Chord ring to call this function.\n\n")
			} else {
				if(len(parts) != 1) {
					print("Too many arguments.\n\n")
				} else {
					var response Node
					var args []string

					if err := call(defaultHost + ":" + port, "Node.Dump", &args, &response); err != nil {
						log.Printf("Client call _Dump_: %v", err)
					} else {
						//addresses
						print("-----Address-----\n" + response.Address + "\n\n")
				
						//predecessor
						print("-----Predecessor-----\n" + response.Predecessor + "\n\n")

						//successors
						print("-----Successors-----\n")
						for i := 0; i < len(response.Successors); i++ {
							if(response.Successors[i] != "") {
								print(response.Successors[i] + "\n")
							}
						}
						print("\n")

						//FingerTable
						print("-----FingerTable-----\n")

						//Do not display redundant information
						for i := 159; i >= 0; i-- {
							if i == 159  && response.FingerTable[i] != "" {
								print(strconv.Itoa(i) + ": " + response.FingerTable[i] + "\n")
								//print("1\n")
								continue
							}
							if response.FingerTable[i] == response.FingerTable[i + 1] || response.FingerTable[i] == "" {
								//print("3\n")
								continue
							} else {
								//print("4\n")
								print(strconv.Itoa(i) + ": " + response.FingerTable[i] + "\n")
							}
						//print(strconv.Itoa(i) + ": " + response.FingerTable[i] + "\n")
						}
						print("\n")

						//bucket
						print("-----Bucket-----\n")
						for key, value := range response.Bucket {
							print("KEY: " + key + " VALUE: " + value + "\n")
						}
						print("\n")
					}
				}
			}
		default:
			print("\nUnrecogized command\n")
			print("Type 'help' to see available commands\n\n")
		}
	}
	if err := scanner.Err(); err != nil {
		log.Fatalf("Scanner error: %v", err)
	}
}
