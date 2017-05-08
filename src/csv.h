/*
 * Sampe Usage:
 * 
 * std::ifstream       file("input.csv");
 *
 * for(CSVIterator loop(file); loop != CSVIterator(); ++loop)
 * {
 *		std::cout << "4th Element(" << (*loop)[3] << ")\n";
 * }
 *
 *
 */


#ifndef CSV_H_
#define CSV_H_

#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

class CSVRow {
public:
	std::string const& operator[](std::size_t index) const;
	std::size_t size() const;

	friend std::istream& operator >> (std::istream& str, CSVRow& data);
	friend std::ostream& operator << (std::ostream& str, const CSVRow& data);

private:
	std::vector<std::string>    m_data;
};




class CSVIterator {
public:
	typedef std::input_iterator_tag     iterator_category;
	typedef CSVRow                      value_type;
	typedef std::size_t                 difference_type;
	typedef CSVRow*                     pointer;
	typedef CSVRow&                     reference;

	CSVIterator(std::istream& str);
	CSVIterator();

	// Pre Increment
	CSVIterator& operator++();

	// Post increment
	CSVIterator operator++(int);

	CSVRow const& operator*()   const;
	CSVRow const* operator->()  const;

	bool operator==(CSVIterator const& rhs);
	bool operator!=(CSVIterator const& rhs);
private:
	std::istream*       m_str;
	CSVRow              m_row;
};


#endif