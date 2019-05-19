// Program Name: Lab 3
//
// Description:  
//  This lab will use multithreading to move through different lanes(queues).
//  The queues contain a cart with items which are processed.
//  Each lane has a lock on its process but when it is finished will move to 
//  another threaded lane.
//
// What's on your mind about this lab?
//  This lab took a lot of downtime thinking how to modify the Lab 3 to
//  implement multithreading. 
//  It took some time tothink about how to implement the locks and initialize the threads.
//
// Author: Jason Yoon
//
// Date: 11/30/2018
//
// IDE Used: Visual Microsoft
//


#include <string>
#include <fstream>
#include <iostream>		// std::cout
#include <sstream>
#include <iomanip>
#include <list>
#include <regex>
#include <stack>
#include <queue>
#include <unordered_map> 
#include <memory>
#include <algorithm>
#include <iterator>
#include <cctype>
#include <thread>
#include <mutex>

using namespace std;


class XML_File_Reader { // Reads Carts
public:
	XML_File_Reader() {};
	void readCartFile(string FILE_NAME);
};

class _XML_File_Reader_Products { // Reads Products
public:
	_XML_File_Reader_Products() {};
	void readProductFile(string FILE_NAME);
};

class threeOfNine {
private:
	unordered_map<string, string> converter;
public:
	threeOfNine();
	template<class T> void doConverstion_Name (T cart);
};

template <class T>
class Item {
public:
	Item() {};
	T itemNum;
	Item(T num) { itemNum = num; }
	T getItemnum() { return itemNum; }
	void resetItem(string hash) { itemNum = hash; }
};


template <class T>
class Cart {
private:
	T cartnum;
	double price;
	string name;
	list<shared_ptr<Item<T>>> items_Carts; // items in cart
public:
	Cart() {};
	Cart(string num) { cartnum = num; }
	void setCartnum(string num) { cartnum = num; }
	void _setListItems(shared_ptr<Item<T>> arg) { items_Carts.push_back(arg); } // make list of items
	string getCartnum() { return cartnum; }
	T getItemnum() { return items_Carts.back()->getItemnum(); }
	list<shared_ptr<Item<T>>> getListofItems() { return items_Carts; }
	void setName(string name) { this.name = name; }
	void setPrice(double price) { this.price = price; }
	string getName() { return name; }
	double getPrice() { return price; }
};

class Product {
private:
	string product;
	double price;
public:
	Product(string name) { product = name; }
	void setProduct(string name) { product = name; }
	void setPrice(string price) { this->price = stod(price); }
	string getProduct() { return product; }
	double getPrice() { return price; }
	bool operator()(const shared_ptr<Product> a, const shared_ptr<Product> b) const {
		return a->getProduct() < b->getProduct();
	}
};

class CompletedProduct {
public:
	CompletedProduct() {};
	string cartnum, name;
	double price = 0.0;
	double getprice() { return price; }
	void setprice(double num) { price = num; }
};

list<shared_ptr<Cart<string>>> listOfCarts; 
vector<queue<shared_ptr<Cart<string>>>> vecOfLanes(10);
unordered_map<string, shared_ptr<CompletedProduct>> helper;
mutex mtx;


void printCompletedCarts(shared_ptr<Cart<string>> cart);
void threadOperations(queue<shared_ptr<Cart<string>>> lane);


/*------------------------------------------ HERE IS MY MAIN ------------------------------------------*/

int main() {
	cout << "Running program. Stay awhile and listen..." << endl;
	string FILE_NAME = "Carts.xml";
	string _FILE_NAME = "Products.xml";
	XML_File_Reader Cartreader;
	_XML_File_Reader_Products Productreader;
	Cartreader.readCartFile(FILE_NAME);
	Productreader.readProductFile(_FILE_NAME);


	thread t1(threadOperations, vecOfLanes.at(0));
	thread t2(threadOperations, vecOfLanes.at(1));
	thread t3(threadOperations, vecOfLanes.at(2));
	thread t4(threadOperations, vecOfLanes.at(3));
	thread t5(threadOperations, vecOfLanes.at(4));
	thread t6(threadOperations, vecOfLanes.at(5));
	thread t7(threadOperations, vecOfLanes.at(6));
	thread t8(threadOperations, vecOfLanes.at(7));
	thread t9(threadOperations, vecOfLanes.at(8));
	thread t10(threadOperations, vecOfLanes.at(9));

	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	t6.join();
	t7.join();
	t8.join();
	t9.join();
	t10.join();

	cin.get();
	return 0;

}

void threadOperations(queue<shared_ptr<Cart<string>>> lane) {
	threeOfNine example;

	while (lane.size() != 0) {
		shared_ptr<Cart<string>> cart = lane.front();
		mtx.lock();
		example.doConverstion_Name(cart);
		printCompletedCarts(cart);
		mtx.unlock();
		lane.pop();
	}

}

void printCompletedCarts(shared_ptr<Cart<string>> cart) { // Now only prints contents of one cart
	shared_ptr<CompletedProduct> completedCart;

	double total = 0.0;
	auto items = cart->getListofItems();
	cout << "Cart # " << cart->getCartnum() << endl;
	for (auto x : items) {
		string container = x->getItemnum();
		transform(container.begin(), container.end(), container.begin(), ::tolower);
		if (helper.find(container) == helper.end())
			cout << container << " not found\n";
		else {
			completedCart = helper[container];
			cout << "Item: " << setw(15) << left << completedCart->name << "     Price: ";
			cout << completedCart->price << endl;
			total += completedCart->price;
		}
	}

	cout << "TOTAL: " << total << endl << endl;
}

template <class T>
void threeOfNine::doConverstion_Name (T cart) { // does converstions using hash
	auto items = cart->getListofItems();
	for (auto x : items) {
		string str = "";
		string container, second;
		container = x->getItemnum();
		while (container.length() != 0) {
			second = container.substr(0, 9); // splitting strings
			str += converter[second]; // the second holds the actual key
			container.erase(0, 9); // erases the "container"
		}
		x->resetItem(str); // puts in value here
	}
}

threeOfNine::threeOfNine() {
	converter["011000100"] = " ";
	converter["010000101"] = "-";
	converter["010001010"] = "+";
	converter["010101000"] = "$";
	converter["000101010"] = "%";
	converter["010010100"] = "*";
	converter["110000100"] = ".";
	converter["010100010"] = "/";
	converter["000110100"] = "0";
	converter["100100001"] = "1";
	converter["001100001"] = "2";
	converter["101100000"] = "3";
	converter["000110001"] = "4";
	converter["100110000"] = "5";
	converter["001110000"] = "6";
	converter["000100101"] = "7";
	converter["100100100"] = "8";
	converter["001100100"] = "9";
	converter["100001001"] = "A";
	converter["001001001"] = "B";
	converter["101001000"] = "C";
	converter["000011001"] = "D";
	converter["100011000"] = "E";
	converter["001011000"] = "F";
	converter["000001101"] = "G";
	converter["100001100"] = "H";
	converter["001001100"] = "I";
	converter["000011100"] = "J";
	converter["100000011"] = "K";
	converter["001000011"] = "L";
	converter["101000010"] = "M";
	converter["000010011"] = "N";
	converter["100010010"] = "O";
	converter["001010010"] = "P";
	converter["000000111"] = "Q";
	converter["100000110"] = "R";
	converter["001000110"] = "S";
	converter["000010110"] = "T";
	converter["110000001"] = "U";
	converter["011000001"] = "V";
	converter["111000000"] = "W";
	converter["010010001"] = "X";
	converter["110010000"] = "Y";
	converter["011010000"] = "Z";
}


void XML_File_Reader::readCartFile(string FILE_NAME) {
	ifstream in(FILE_NAME);
	string line;
	int counter = 0;
	auto start = "<"s;
	auto end = ">"s;
	auto start_ex = ">"s;
	auto end_ex = "</"s;
	regex base_regex(start + "(.*)" + end);

	regex _startOrEnd("(.*)(XMLCarts)"); // skip the XMLCart
	regex _cartNum("(Cart)(.*)"); // Get the Cart Num
	regex _pullOut_cartNum("[a-zA-Z]*(.*)");

	regex cart_item("(.*)(>)(.*)"); // Get the Item Number
	regex cart_contents("[a-z0-9]*>(.*)</[a-z0-9]*"); //regex this --> "itemXX> 101010101010101 </itemXX"

	smatch base_match;
	string matched;
	cout << "Reading cart file..." << endl;
	while (getline(in, line)) {
		string tmp; // strip whitespaces from the beginning
		for (unsigned int i = 0; i < line.length(); i++)
		{
			if (line[i] == '\t' && tmp.size() == 0)
			{
			}
			else
			{
				tmp += line[i];
			}
		}
		regex_match(tmp, base_match, base_regex); // take away < and >
		matched = base_match[1].str();
		if (regex_match(matched, _cartNum)) { // works
			regex_match(matched, base_match, _pullOut_cartNum); // get the actual cart number
			matched = base_match[1].str();
			shared_ptr<Cart<string>> newCart(new Cart<string>(matched));
			listOfCarts.push_back(newCart);
		}
		else if (regex_match(matched, cart_item))
		{
			regex_match(matched, base_match, cart_contents);
			matched = base_match[1].str();
			shared_ptr<Item<string>> newItem(new Item<string>(matched));
			listOfCarts.back()->_setListItems(newItem);
		}
		else if (regex_match(matched, _startOrEnd)) // for XMLCarts tag at start and end
		{
			continue;
		}
	}
	in.close();

	int _counter = 0;
	for (auto cart : listOfCarts) {
		vecOfLanes.at(_counter).push(cart);
		_counter++;
		if (_counter == 10)
			_counter = 0;
	}

	cout << "Finished reading cart file..." << endl;
	cout << "Carts put into 10 lanes (queues) to be processed..." << endl;

}

// Create HashMap from Product file - data called CompletedProduct
void _XML_File_Reader_Products::readProductFile(string FILE_NAME) {
	ifstream in(FILE_NAME);
	string line;
	auto start = "<"s;
	auto end = ">"s;
	auto start_ex = "Name>"s;
	auto end_ex = "</Name"s;
	regex base_regex(start + "(.*)" + end);

	regex _startOrEnd("(.*)(BarcodeList)"); // skip the Header
	regex _productName("(Name)>(.*)</(Name)");
	regex _pullOut_Product("[a-zA-Z0-9]*>(.*)</[a-zA-Z0-9]*");

	regex product_price("(Price)>(.*)</(Price)"); // Get the Item Number
	regex price_contents("[a-zA-Z0-9]*>(.*)</[a-zA-Z0-9]*"); //regex this --> "itemXX> 101010101010101 </itemXX"

	smatch base_match;
	string matched;

	string pleaseWork = "";
	double pleaseWorkMore = 0.0;
	list<shared_ptr<CompletedProduct>> listOfCompletes;


	cout << "Reading product file..." << endl;
	while (getline(in, line)) {
		shared_ptr<CompletedProduct> container(new CompletedProduct);
		string tmp; // strip whitespaces from the beginning
		for (unsigned int i = 0; i < line.length(); i++)
		{
			if (line[i] == '\t' && tmp.size() == 0)
			{
			}
			else
			{
				tmp += line[i];
			}
		}
		regex_match(tmp, base_match, base_regex); // take away < and >
		matched = base_match[1].str();

		if (regex_match(matched, _productName)) {
			regex_match(matched, base_match, _pullOut_Product);
			matched = base_match[1].str();
			//shared_ptr<Product> newProduct(new Product(matched));
			listOfCompletes.push_back(container);
			container->name = matched;
		}
		else if (regex_match(matched, product_price))
		{
			regex_match(matched, base_match, price_contents);
			matched = base_match[1].str();
			//cout << matched << endl;
			pleaseWorkMore = stod(matched);
			listOfCompletes.back()->setprice(pleaseWorkMore);
			//cout << pleaseWorkMore << endl;
			//cout << container->price << endl;
		}
		else if (regex_match(matched, _startOrEnd))
		{
			continue;
		}
		pleaseWork = container->name;
		//container->setprice(pleaseWorkMore);
		transform(pleaseWork.begin(), pleaseWork.end(), pleaseWork.begin(), ::tolower);
		pleaseWork = pleaseWork.substr(0, 5);
		helper[pleaseWork] = container; // HashMap from Product file - data called CompletedProduct

	}
	in.close();
	cout << "Finished reading product file..." << endl;

}
