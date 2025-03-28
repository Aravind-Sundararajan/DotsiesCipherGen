#ifndef KVP_H_
#define KVP_H_
#include <bitset>
#include <utility>

class kvp // Key Value Pair
{
public:
	// Default constructor - initializes with default values
	kvp() noexcept : key('a'), value() {}
	
	// Value constructor - initializes with provided key and value
	kvp(char k, std::bitset<5> v) noexcept : key(k), value(v) {}
	
	// Copy constructor
	kvp(const kvp& other) noexcept = default;
	
	// Move constructor
	kvp(kvp&& other) noexcept = default;
	
	// Copy assignment operator
	kvp& operator=(const kvp& other) noexcept = default;
	
	// Move assignment operator 
	kvp& operator=(kvp&& other) noexcept = default;
	
	// Destructor
	~kvp() = default;
	
	// Getters
	char get_key() const noexcept { return key; }
	const std::bitset<5>& get_value() const noexcept { return value; }
	
	// Setters
	void set_key(char k) noexcept { key = k; }
	void set_value(const std::bitset<5>& v) noexcept { value = v; }
	
	// Comparison operators
	bool operator==(const kvp& other) const noexcept {
		return key == other.key && value == other.value;
	}
	
	bool operator!=(const kvp& other) const noexcept {
		return !(*this == other);
	}
	
	// Data members
	char key;
	std::bitset<5> value;
};

#endif // KVP_H_