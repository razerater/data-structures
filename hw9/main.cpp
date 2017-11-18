#include <iostream> // iostream includes string
#include <fstream>
#include <vector>

// a simple class that stores two variables:
// key -- sequence of k characters in the genome
// positions -- vector of integers that represent locations in the genome that the kmers appear
class Kmer {
public:
	// we must implement a default constructor for when the table (vector of Kmer objects) is resized
	// the empty string is used to signify that this Kmer is empty/not being used
	Kmer() : key("") {}
	std::string key;
	std::vector<int> positions;
};

unsigned int hash_func(const std::string& key) {
	// this implementation comes from
	// http://www.partow.net/programming/hashfunctions/
	unsigned int hash = 1315423911;
	for (unsigned int i = 0; i < key.size(); i++) {
		hash ^= ((hash << 5) + key[i] + (hash >> 2));
	}
	return hash;
}

void rehash_table(std::vector<Kmer>& table, int& table_size) {
	// double the size of the table
	table_size *= 2;
	// create a new table and relocate all kmers in the old table to the new one, which we eventually
	// will assign back to the table vector
	std::vector<Kmer> new_table(table_size);
	for (unsigned int i = 0; i < table.size(); i++) {
		if (table[i].key != "") {
			// if we find a non-empty kmer, find an index/location for it in the new table
			unsigned int hash = hash_func(table[i].key);
			for (int j = 1; true; j++) {
				// use quadratic probing until we find an empty location at which to insert the kmer
				// or until we find a space already occupied by the key we want to insert, in which
				// case we just escape the loop
				// additionally, we know that there will always be space in the table for a new kmer
				// because we resize the table whenever the occupancy is greater than a certain level
				unsigned int index = (hash+j*j)%table_size;
				if (new_table[index].key == "") {
					// insert the kmer
					new_table[index].key = table[i].key;
					new_table[index].positions = table[i].positions;
					break;
				}
				if (new_table[index].key == table[i].key) {
					// no need to insert if the table already contains an identical kmer
					break;
				}
			}
		}
	}
	// finally, assign the table we created to the old table (which we passed in by reference)
	table = new_table;
}

int main() {
	std::ifstream genome_file;
	std::string whole_file = "";
	std::string str;
	int table_size = 100; // default table size is 100
	int entries = 0; // increment this every time we add a new key, then use it to calculate occupancy
	int k = 0;
	float max_occupancy = 0.5;
	std::vector<Kmer> table(table_size);

	while (std::cin >> str) {
		if (str == "genome") {
			std::cin >> str;
			genome_file.open(str.c_str());
		}
		else if (str == "table_size") {
			std::cin >> table_size;
			table.resize(table_size);
		}
		else if (str == "occupancy") {
			std::cin >> max_occupancy;
		}
		else if (str == "kmer") {
			std::cin >> k;
			// read the genome file into a string
			while (genome_file >> str) {
				whole_file += str;
			}
			for (unsigned int i = 0; i+k < whole_file.size(); i++) {
				// make a key by getting k characters from the file, starting from i
				std::string new_key = whole_file.substr(i, k);
				unsigned int hash = hash_func(new_key);
				for (int j = 1; true; j++) {
					// use quadradic probing to find the index at which we should insert the key
					// (j+1)%N, (j+4)%N, (j+9)%N, (j+16)%N, ...
					unsigned int index = (hash+j*j)%table_size;
					if (table[index].key == "") {
						// if the kmer at the hashed location is empty then we give it data
						table[index].key = new_key;
						table[index].positions.push_back(i);
						entries++; // we just added a new kmer to the table, so increment entries
						if ((float)entries/table_size > max_occupancy) {
							// if the occupancy is over the max, then we need to resize the table
							// and rehash the entries
							rehash_table(table, table_size);
						}
						break;
					}
					if (table[index].key == new_key) {
						// if the found kmer's key matches the one we want to insert then we just add
						// just add its location in the genome to positions and continue
						table[index].positions.push_back(i);
					}
				}
			}
			// a bit of code that helped me in visualizing the hash table
			// prints out the entire contents of the hash table, including the contents of each Kmer
			// object's position vector
			/*for (int i = 0; i < table_size; i++) {
				std::cout << table[i].key;
				for (unsigned int j = 0; j < table[i].positions.size(); j++) {
					std::cout << " " << table[i].positions[j];
				}
				std::cout << std::endl;
			}*/
		}
		else if (str == "query") {
			int max_mms; // max_mms is the max allowed number of mismatches
			std::string query; // query string input by user
			std::cin >> max_mms >> query;
			std::string key = query.substr(0, k); // key to search the hash table for
			unsigned int hash = hash_func(key);
			unsigned int index;
			bool match_found = false; // so we know if we need to print "No match."
			for (int i = 1; true; i++) {
				// again, we use quadratic probing to get the index of the kmer
				// note that this loop will generally only run one to a few times before it is escaped
				index = (hash+i*i)%table_size;
				if (table[index].key == key) {
					// if we found what we were looking for then break out of the loop
					break;
				}
			}
			// std::cout << index << std::endl;
			std::cout << "Query: " << query << std::endl;
			for (unsigned int i = 0; i < table[index].positions.size(); i++) {
				int mms = 0; // mms is the actual number of mismatches
				// found_query is a string containing one of the kmers we found by searching the table
				std::string found_query = whole_file.substr(table[index].positions[i], query.size());
				// we can start the comparison between query and found_query at index k because we
				// know that the two string should be the same up until that point
				for (unsigned int j = k; j < query.size(); j++) {
					if (query[j] != found_query[j]) {
						// increment mms for each mismatch between characters in the two strings at
						// the same index
						++mms;
					}
				}
				if (mms <= max_mms) {
					// we found a match if query and found_query are similar within max_mms mismatches
					// set match_found to true in this case, and print out info
					match_found = true;
					std::cout << table[index].positions[i] << " " << mms << " "
							  << found_query << std::endl;
				}
			}
			if (!match_found) {
				std::cout << "No Match" << std::endl;
			}
		}
		else if (str == "quit") {
			break;
		}
		else {
			// this should never be the case, but why not include it
			std::cout << "Invalid input. Type \"quit\" to exit the program." << std::endl;
		}
	}

	return 0;
}
