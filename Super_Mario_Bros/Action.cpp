#include "Action.h"

std::string& Action::getName()
{
	return m_name;
}

const std::string& Action::getName() const
{
	return m_name;
}

std::string& Action::getType()
{
	return m_type;
}

const std::string& Action::getType() const
{
	return m_type;
}