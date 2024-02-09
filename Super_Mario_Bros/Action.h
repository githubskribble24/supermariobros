#pragma once

#include <iostream>

class Action
{
	std::string m_name;
	std::string m_type;

public:
	Action(const std::string& name, const std::string& type) : m_name(name), m_type(type) {}

	std::string& getName();
	const std::string& getName() const;
	const std::string& getType() const;
	std::string& getType();

};