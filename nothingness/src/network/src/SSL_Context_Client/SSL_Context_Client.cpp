#include "SSL_Context_Client.h"
#include <wolfssl/ssl.h>
#include "..//..//network.h"

nothingness::network::SSL::Context_client::Context_client(){
	ctx = wolfSSL_CTX_new(wolfTLSv1_2_client_method());

	if (ctx == NULL) {
		this->Throw(new nothingness::nothingness_exception("failed to create WOLFSSL_CTX for client!"));
	}
}
