#pragma once
#ifndef SERVER_HEADER_H
#define SERVER_HEADER_H

#pragma region INCLUDES
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <functional>
#include <thread>
#include <string>
#pragma endregion INCLUDES

using boost::asio::ip::tcp;

#pragma region CLASS_CLIENT
namespace MatthewsNamespace {
	class ServerClass {
	protected:
		// Create an I/O context

		boost::asio::io_context io_context; // A context needed for the library to work
		boost::asio::ip::tcp::acceptor* acceptor; // An acceptor that accepts incoming connections
		std::thread* MainServerThread;
		long long _port_num;
		std::string MessageSent;

	public:
		ServerClass(long long PortNumber, std::string Message) : _port_num(PortNumber), MessageSent(Message.c_str()) {
			try {
				acceptor = new boost::asio::ip::tcp::acceptor(io_context, tcp::endpoint(tcp::v4(), _port_num));
				// We are going to initialize a working thread to be active all the time
				MainServerThread = new std::thread([&]() -> void {
					try {
						while (true) {
							std::cout << "Accepting Connections on port " << _port_num << "...\n";
							tcp::socket socket(io_context); // A socket to catch connections
							acceptor->accept(socket);
							boost::system::error_code error;
							std::cout << "Connection established on port " << _port_num << "!\n";

							long long currentmessagelength = 0;
							while (socket.is_open()) { // While a connection is established		
								currentmessagelength += boost::asio::write(socket, boost::asio::buffer(MessageSent), error);
								if (currentmessagelength == MessageSent.length()) {
									break;
								}
								// std::cout << "Message sent: " << MessageSent << "\n";
							}
							std::cout << "\nConnection interrupted successfully (socket closed)\n";
						}

					}
					catch (std::exception& E) {
						std::cerr << E.what() << "\n";
					}
					}
				);
				// Lambda thread functor ends here
			}
			catch (std::exception& E) {
				std::cerr << E.what() << "\n";
			}
		};

	};

	bool port_is_open(const std::string& address, int port) // Checks for open ports
	{
		sf::TcpSocket socket;
		bool open = (socket.connect(sf::IpAddress(address), port) == sf::Socket::Done);
		socket.disconnect();
		return open;
	}
};
#pragma endregion CLASS_CLIENT



#endif
