#include "csv.h"

std::string const& CSVRow::operator[](std::size_t index) const
{
	return m_data[index];
}

std::size_t CSVRow::size() const
{
	return m_data.size();
}


std::istream& operator >> (std::istream& str, CSVRow& data)
{

	std::string         line;
	std::getline(str, line);

	std::stringstream   lineStream(line);
	std::string         cell;

	data.m_data.clear();
	while (std::getline(lineStream, cell, ','))
	{
		data.m_data.push_back(cell);
	}
	// This checks for a trailing comma with no data after it.
	if (!lineStream && cell.empty())
	{
		// If there was a trailing comma then add an empty element.
		data.m_data.push_back("");
	}

	return str;
}

std::ostream & operator<<(std::ostream & str, const CSVRow & data)
{
	std::vector<std::string>::const_iterator It = data.m_data.cbegin();
	if (It != data.m_data.cend()) {
		str << *It;
		++It;
		for (; It != data.m_data.cend(); ++It)
			str << ','<< *It;	
	}

	return str;
}



CSVIterator::CSVIterator(std::istream& str) :m_str(str.good() ? &str : NULL) {
	++(*this);
}

CSVIterator::CSVIterator() : m_str(NULL) {}



// Pre Increment
CSVIterator& CSVIterator::operator++() {
	if (m_str) {
		if (!((*m_str) >> m_row))
		{
			m_str = NULL;
		}
	}return *this;
}

// Post increment
CSVIterator CSVIterator::operator++(int) {
	CSVIterator    tmp(*this);
	++(*this);
	return tmp;
}

CSVRow const& CSVIterator::operator*()   const {
	return m_row;
}
CSVRow const* CSVIterator::operator->()  const {
	return &m_row;
}

bool CSVIterator::operator==(CSVIterator const& rhs) {
	return ((this == &rhs) || ((this->m_str == NULL) && (rhs.m_str == NULL)));
}
bool CSVIterator::operator!=(CSVIterator const& rhs) {
	return !((*this) == rhs);
}