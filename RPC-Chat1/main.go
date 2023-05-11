package main

import (
	"bufio"
	"encoding/binary"
	"fmt"
	"io"
	"log"
	"net"
	"os"
	"sort"
	"strings"
	"sync"
	"time"
	"unicode"
)

const (
	MsgRegister = iota
	MsgList
	MsgCheckMessages
	MsgTell
	MsgSay
	MsgQuit
	MsgShutdown
)

var mutex sync.Mutex
var messages map[string][]string
var shutdown chan struct{}

// SERIALIZING
func WriteUint16(w io.Writer, i uint16) error {
	buf := make([]byte, 2)
	binary.BigEndian.PutUint16(buf, i)
	_, err := w.Write(buf)
	return err
}

func ReadUint16(r io.Reader) (uint16, error) {
	buf := make([]byte, 2)
	_, err := io.ReadFull(r, buf)
	if err != nil {
		return 0, err
	}
	return binary.BigEndian.Uint16(buf), nil
}

func WriteString(w io.Writer, s string) error {
	err := WriteUint16(w, uint16(len(s)))
	if err != nil {
		return err
	}
	_, err = w.Write([]byte(s))
	return err
}

func ReadString(r io.Reader) (string, error) {
	length, err := ReadUint16(r)
	if err != nil {
		return "", err
	}
	buf := make([]byte, length)
	_, err = io.ReadFull(r, buf)
	if err != nil {
		return "", err
	}
	return string(buf), nil
}

func WriteStringSlice(w io.Writer, ss []string) error {
	err := WriteUint16(w, uint16(len(ss)))
	if err != nil {
		return err
	}
	for _, s := range ss {
		err = WriteString(w, s)
		if err != nil {
			return err
		}
	}
	return nil
}

func ReadStringSlice(r io.Reader) ([]string, error) {
	length, err := ReadUint16(r)
	if err != nil {
		return nil, err
	}
	ss := make([]string, length)
	for i := range ss {
		s, err := ReadString(r)
		if err != nil {
			return nil, err
		}
		ss[i] = s
	}
	return ss, nil
}

// SERVER
func server(listenAddress string) {
	shutdown = make(chan struct{})
	messages = make(map[string][]string)

	// set up network listen and accept loop here
	// to receive and dispatch RPC requests
	// .

	ln, err := net.Listen("tcp", listenAddress)
	if err != nil {
		print("error in listen")
		print(err)
		Shutdown()
	}
	go func() {
		for {
			conn, err := ln.Accept()
			if err != nil {
				print("error in accept")
				print(err)
				Shutdown()
			}
			i, _ := ReadUint16(conn)
			switch i {
			case 0:
				s, _ := ReadString(conn)
				Register(s)
			case 1:
				ss := List()
				WriteStringSlice(conn, ss)
			case 2:
				s, _ := ReadString(conn)
				ss := CheckMessages(s)
				WriteStringSlice(conn, ss)
			case 3:
				ss, _ := ReadStringSlice(conn)
				Tell(ss[0], ss[1], ss[2])
			case 4:
				ss, _ := ReadStringSlice(conn)
				Say(ss[0], ss[1])
			case 5:
				s, _ := ReadString(conn)
				Quit(s)
			case 6:
				Shutdown()
			}
			defer conn.Close()
		}
	}()

	// wait for a shutdown request
	<-shutdown
	time.Sleep(100 * time.Millisecond)
	print("Shutting down")
}

func Register(user string) error {
	if len(user) < 1 || len(user) > 20 {
		return fmt.Errorf("Register: user must be between 1 and 20 letters")
	}
	for _, r := range user {
		if !unicode.IsLetter(r) && !unicode.IsDigit(r) {
			return fmt.Errorf("Register: user must only contain letters and digits")
		}
	}
	mutex.Lock()
	defer mutex.Unlock()

	msg := fmt.Sprintf("*** %s has logged in", user)
	log.Printf(msg)
	for target, queue := range messages {
		messages[target] = append(queue, msg)
	}
	messages[user] = nil

	return nil
}

func List() []string {
	mutex.Lock()
	defer mutex.Unlock()

	var users []string
	for target := range messages {
		users = append(users, target)
	}
	sort.Strings(users)

	return users
}

func CheckMessages(user string) []string {
	mutex.Lock()
	defer mutex.Unlock()

	if queue, present := messages[user]; present {
		messages[user] = nil
		return queue
	} else {
		return []string{"*** You are not logged in, " + user}
	}
}

func Tell(user, target, message string) {
	mutex.Lock()
	defer mutex.Unlock()

	msg := fmt.Sprintf("%s tells you %s", user, message)
	if queue, present := messages[target]; present {
		messages[target] = append(queue, msg)
	} else if queue, present := messages[user]; present {
		messages[user] = append(queue, "*** No such user: "+target)
	}
}

func Say(user, message string) {
	mutex.Lock()
	defer mutex.Unlock()

	msg := fmt.Sprintf("%s says %s", user, message)
	for target, queue := range messages {
		messages[target] = append(queue, msg)
	}
}

func Quit(user string) {
	mutex.Lock()
	defer mutex.Unlock()

	msg := fmt.Sprintf("*** %s has logged out", user)
	log.Print(msg)
	for target, queue := range messages {
		messages[target] = append(queue, msg)
	}
	delete(messages, user)
}

func Shutdown() {
	shutdown <- struct{}{}
}

// CLIENT
func client(serverAddress string, username string) {
	stop := make(chan bool)
	conn, err := net.Dial("tcp", serverAddress)
	if err != nil {
		log.Fatalf("Failed to dial server")
	}
	RegisterClient(username, serverAddress)
	go func() {
		for {
			select {
			default:
				messages, _ := CheckMessagesClient(username, serverAddress)
				for _, msg := range messages {
					fmt.Println(msg)
				}
				time.Sleep(time.Second)
			case <-stop:
				fmt.Println("Quitting...")
				return
			}

		}
	}()
	conn.Close()
	scanner := bufio.NewScanner(os.Stdin)
	for scanner.Scan() {
		msg := scanner.Text()
		command := strings.Split(msg, " ")[0]
		switch command {
		case "tell":
			TellClient(username, msg, serverAddress)
		case "say":
			SayClient(username, msg, serverAddress)
		case "list":
			usernames, _ := ListClient(serverAddress)
			for _, un := range usernames {
				fmt.Println(un)
			}
		case "quit":
			stop <- true
			QuitClient(username, serverAddress)
			return
		case "help":
			help()
		case "shutdown":
			ShutdownClient(serverAddress)
		case "":
			continue
		default:
			fmt.Println("Unrecognized command!!!!!")
			help()
		}

	}
	if err := scanner.Err(); err != nil {
		log.Fatal("error scanning input:", err)
	}
}

func RegisterClient(username string, serverAddress string) {
	conn, err := net.Dial("tcp", serverAddress)
	if err != nil {
		log.Fatalf("Failed to dial server")
	}
	defer conn.Close()
	WriteUint16(conn, 0)
	WriteString(conn, username)
}

func ListClient(serverAddress string) ([]string, error) {
	conn, err := net.Dial("tcp", serverAddress)
	if err != nil {
		log.Fatalf("Failed to dial server")
	}
	defer conn.Close()
	WriteUint16(conn, 1)
	return ReadStringSlice(conn)
}

func CheckMessagesClient(username string, serverAddress string) ([]string, error) {
	conn, err := net.Dial("tcp", serverAddress)
	if err != nil {
		log.Fatalf("Failed to dial server")
	}
	defer conn.Close()
	WriteUint16(conn, 2)
	WriteString(conn, username)
	return ReadStringSlice(conn)
}

func TellClient(username string, message string, serverAddress string) {
	conn, err := net.Dial("tcp", serverAddress)
	if err != nil {
		log.Fatalf("Failed to dial server")
	}
	defer conn.Close()
	msg_split := strings.Split(message, " ")
	target := msg_split[1]
	msg := ""
	for i := 2; i < len(msg_split)-1; i++ {
		msg += msg_split[i] + " "
	}
	msg += msg_split[len(msg_split)-1]
	WriteUint16(conn, 3)
	ss := []string{username, target, msg}
	WriteStringSlice(conn, ss)
}

func SayClient(username string, message string, serverAddress string) {
	conn, err := net.Dial("tcp", serverAddress)
	if err != nil {
		log.Fatalf("Failed to dial server")
	}
	defer conn.Close()
	msg_split := strings.Split(message, " ")
	msg := ""
	for i := 1; i < len(msg_split)-1; i++ {
		msg += msg_split[i] + " "
	}
	msg += msg_split[len(msg_split)-1]
	WriteUint16(conn, 4)
	ss := []string{username, msg}
	WriteStringSlice(conn, ss)
}

func QuitClient(username string, serverAddress string) {
	conn, err := net.Dial("tcp", serverAddress)
	if err != nil {
		log.Fatalf("Failed to dial server")
	}
	defer conn.Close()
	WriteUint16(conn, 5)
	WriteString(conn, username)
}

func ShutdownClient(serverAddress string) {
	conn, err := net.Dial("tcp", serverAddress)
	if err != nil {
		log.Fatalf("Failed to dial server")
	}
	defer conn.Close()
	WriteUint16(conn, 6)
}

func help() {
	fmt.Println("tell <user> some message) This sends “some message” to a specific user.")
	fmt.Println("say some other message) This sends “some other message” to all users.")
	fmt.Println("list) This lists all users currently logged in.")
	fmt.Println("quit) this logs you out.")
	fmt.Println("shutdown) this shuts down the server.")
	fmt.Println("help) list all of the recognized commands.")
}

func main() {
	log.SetFlags(log.Ltime)

	var listenAddress string
	var serverAddress string
	var username string

	switch len(os.Args) {
	case 2:
		listenAddress = net.JoinHostPort("", os.Args[1])
	case 3:
		serverAddress = os.Args[1]
		if strings.HasPrefix(serverAddress, ":") {
			serverAddress = "localhost" + serverAddress
		}
		username = strings.TrimSpace(os.Args[2])
		if username == "" {
			log.Fatal("empty user name")
		}
	default:
		log.Fatalf("Usage: %s <port number>   OR   %s localhost:<server port> <username>",
			os.Args[0], os.Args[0])
	}

	if len(listenAddress) > 0 {
		server(listenAddress)
	} else {
		client(serverAddress, username)
	}
}
