#ifndef _NOTHINGNESS_SSL_CONTEXT_H
#define _NOTHINGNESS_SSL_CONTEXT_H

#include <cstdint>
#include "..//..//..//error_handler/error_handler.h"

struct WOLFSSL_CTX;

namespace nothingness {
	namespace network {
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
		}
	}
}

#endif // !1