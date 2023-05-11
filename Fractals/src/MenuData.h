#ifndef _MENU_DATA_
#define _MENU_DATA_

#include "ActionData.h"
#include <map>

typedef void (*ActionFunctionType)(ActionData& action_data);

class MenuData {
	public:
		//constructor
		MenuData();
		
		//methods
		void addAction(const std::string& name, ActionFunctionType func, const std::string& description);
		const std::vector<std::string>& getNames() const;
		ActionFunctionType getFunction(const std::string& name);
		const std::string& getDescription(const std::string& name);

	private:
		//data members
		std::vector<std::string> mNames;
		std::map<std::string, ActionFunctionType> mFunctionMap;
	       	std::map<std::string,std::string> mDescriptionMap;
		
};

#endif
