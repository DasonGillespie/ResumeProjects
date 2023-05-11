#include "MenuData.h"

MenuData::MenuData(){
	
	}

void MenuData::addAction(const std::string& name, ActionFunctionType func, const std::string& description) {
	mNames.push_back(name);
	mFunctionMap[name] = func;
	mDescriptionMap[name] = description;	
}

const std::vector<std::string>& MenuData::getNames() const {
	return mNames;	
}

ActionFunctionType MenuData::getFunction(const std::string& name) {
	if(mFunctionMap.count(name) == 0) {
		return 0;
	} else {
		return mFunctionMap[name];
	}
}

const std::string& MenuData::getDescription(const std::string& name) {
	static std::string no_string = "";
	if(mDescriptionMap.count(name) == 0) {
		return no_string;
	} else {
		return mDescriptionMap[name];
	}
}

