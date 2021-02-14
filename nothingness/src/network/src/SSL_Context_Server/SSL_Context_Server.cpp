#include "SSL_Context_Server.h"
#include <wolfssl/ssl.h>
#include "..//..//network.h"

nothingness::network::SSL::Context_Server::Context_Server(){
	ctx = wolfSSL_CTX_new(wolfTLSv1_2_server_method());

	if (ctx == NULL) {
		this->Throw(new nothingness::nothingness_exception("failed to create WOLFSSL_CTX for server!"));
		return;
	}
}