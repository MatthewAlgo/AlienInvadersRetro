#pragma once
#ifndef CLIENT_HEADER_H
#define CLIENT_HEADER_H
#pragma region INCLUDES
#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <array>
#include <vector>
#pragma endregion INCLUDES

using boost::asio::ip::tcp;

#pragma region CLIENT_CLASS
namespace MatthewsNamespace {
	class ClientClass {
	protected:
		boost::asio::io_context io_context;
		std::unique_ptr<tcp::resolver> resolver;
		std::unique_ptr<tcp::resolver::results_type> endpoints;
		tcp::socket* socket;
		std::vector<unsigned long long> length_of_connections; unsigned long long total_len = 0;

		std::thread* ListenerThread;
		std::string ip_address, service_port;
	private:
		std::string final_message;
	public:
		ClientClass(std::string IP_ADDR, std::string SERVICE_PORT) : ip_address(IP_ADDR), service_port(SERVICE_PORT) {
			try {
				// We initialize the resolver
				resolver = std::make_unique<tcp::resolver>(io_context);
				// We determine the endpoint
				endpoints = std::make_unique<tcp::resolver::results_type>(resolver.get()->resolve(IP_ADDR, SERVICE_PORT));
				// We initialize the socket
				socket = new tcp::socket(io_context);
				boost::asio::connect(*socket, *endpoints.get());

				// Listen for messages
				ListenerThread = new std::thread([&]() -> void {
					try {
						while (socket->is_open()) {
							std::array<char, 128> buffer; // Reads in chunks of 128 bytes
							boost::system::error_code error;
							// While the server can communicate with the client -> the socket is open
							size_t len = socket->read_some(boost::asio::buffer(buffer), error);
							if (error == boost::asio::error::eof) {
								// Connection cut off
								break;
							}
							else if (error)
								throw boost::system::system_error(error);
							total_len += len;

							std::cout << "Message received: ";
							std::cout.write(buffer.data(), len);
							std::cout << "\n";
							final_message += std::string(std::begin(buffer), std::begin(buffer) + len);

						}
						std::cout << "\nConnection interrupted successfully (socket closed). Read " << total_len << " bytes of data\n";
						std::cout << "Final message: " << final_message << "\n";

					}
					catch (std::exception& E) {
						std::cout << "Log message: ";
						std::cerr << E.what() << "\n";
					}}
				);
				// End of listener thread
			}
			catch (std::exception& E) {
				std::cerr << E.what() << "\n";
			}
		};

	};
}
#pragma endregion CLIENT_CLASS
#endif