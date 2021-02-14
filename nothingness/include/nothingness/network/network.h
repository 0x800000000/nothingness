#ifndef NOTHINGNESS_NETWORK_H
#define NOTHINGNESS_NETWORK_H

#include<nothingness/error_handler.h>

#define NOTHINGNESS_SOCKET_INPUT_DATA_USE_IP_PORT 0
#define NOTHINGNESS_SOCKET_INPUT_DATA_USE_DOMAIN_PORT 1
#define NOTHINGNESS_SOCKET_INPUT_DATA_USE_DOMAIN 2
#define NOTHINGNESS_SOCKET_INPUT_DATA_USE_ANY_IP_PORT 3

#ifdef WIN32
#include<winsock2.h>
typedef SOCKET n_socket;
#else
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
typedef uint32_t n_socket;
#endif 
#include <string>

struct sockaddr_in;
struct WOLFSSL_CTX;
struct WOLFSSL;

namespace nothingness {
	namespace network {
		class Socket : public nothingness::error_handler {
		protected:

			n_socket sock;
			sockaddr_in addr;

			bool isClose = false;

		public:

			Socket();

			virtual void _listen(const char* ip, uint16_t port, uint8_t input_data) = 0;
			virtual void _connect(const char* ip, uint16_t port, uint8_t input_data) = 0;

			virtual int write(const char* data, int len) = 0;
			virtual int read(char* data, int len) = 0;

			virtual void create_socket() = 0;

			virtual void _close() = 0;

			virtual Socket* _accept() = 0;

			virtual void operator=(Socket&) = delete;

			~Socket();

		};

		namespace SSL {
			class Context : public nothingness::error_handler {
			protected:

				WOLFSSL_CTX* ctx;

			public:

				Context();

				bool use_certificate_file(const char* ceritficate_file, uint16_t ssl_file_type);
				bool use_PrivateKey_file(const char* PrivateKey_file, uint16_t ssl_file_type);
				bool load_verify_locations(const char* certificate_file, const char*);

				WOLFSSL_CTX* getWolfSSLContext();

				~Context();
			};

			class SSL_Socket : public Socket {

				Context* ctx;

				WOLFSSL* ssl;
				int addrlen;

			public:
				SSL_Socket(Context* ctx);
				SSL_Socket();
				SSL_Socket(Context* ctx, WOLFSSL* ssl, n_socket sock);

				void _listen(const char* ip, uint16_t port, uint8_t input_data);
				void _connect(const char* ip, uint16_t port, uint8_t input_data);

				Socket* _accept();

				int write(const char* data, int len);
				int read(char* data, int len);

				void create_socket();

				void _close();

				~SSL_Socket();

			};

			class Context_client : public Context {

			public:
				Context_client();

			};

			class Context_Server : public Context {

			public:
				Context_Server();

			};

		}

		int initalize_network();
		void shutdown_network();

		nothingness::network::SSL::Context* create_SSL_server_context();
		nothingness::network::SSL::Context* create_SSL_client_context();

		void initalize_network_ssl();
		void shutdown_network_ssl();

		Socket* create_tcp_socket();
		Socket* create_ssl_socket(nothingness::network::SSL::Context* ctx);


		class TCP_Socket : public Socket {

			int addrlen;

		public:

			TCP_Socket();
			TCP_Socket(n_socket sock);

			void _listen(const char* ip, uint16_t port, uint8_t input_data) override;
			void _connect(const char* ip, uint16_t port, uint8_t input_data) override;

			Socket* _accept();

			int write(const char* data, int len) override;
			int read(char* data, int len) override;

			void create_socket() override;

			void _close()  override;

			~TCP_Socket();
		};

	}
}
#endif