tear: ./native-tear/tear.cpp
	g++ -Wall ./native-tear/tear.cpp -o usbwatcher -lboost_system -lboost_filesystem -lcryptopp

decrypter: ./decrypter/decrypter.cpp
	g++ -Wall ./decrypter/decrypter.cpp -o decrypter -lboost_system -lboost_filesystem -lcryptopp
