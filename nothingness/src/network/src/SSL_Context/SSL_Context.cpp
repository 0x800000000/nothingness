#include "SSL_Context.h"
#include <wolfssl/ssl.h>
#include "..//..//network.h"

nothingness::network::SSL::Context::Context() {

}

bool nothingness::network::SSL::Context::use_certificate_file(const char* ceritficate_file, uint16_t ssl_file_type){
	if (wolfSSL_CTX_use_certificate_file(ctx, ceritficate_file, ssl_file_type) != SSL_SUCCESS) {
		std::string error = "failed to load: \"";
		error += ceritficate_file;
		error += "\", please check the file.";
		
		this->Throw(new nothingness::nothingness_exception(error));
		return false;
	}
	
	return true;
}

bool nothingness::network::SSL::Context::use_PrivateKey_file(const char* PrivateKey_file, uint16_t ssl_file_type){
	if (wolfSSL_CTX_use_PrivateKey_file(ctx, PrivateKey_file, ssl_file_type) != SSL_SUCCESS) {
		std::string error = "failed to load: \"";
		error += PrivateKey_file;
		error += "\", please check the file.";

		this->Throw(new nothingness::nothingness_exception(error));
		return false;
	}

	return true;
}

bool nothingness::network::SSL::Context::load_verify_locations(const char* certificate_file, const char* private_key_file){
	int ret = wolfSSL_CTX_load_verify_locations(ctx, certificate_file, private_key_file);
	if (ret != SSL_SUCCESS) {
		std::string error = "failed to load: \"";
		error += certificate_file;
		error += "\", please check the file.";
		this->Throw(new nothingness::nothingness_exception(error));
		return false;
	}

	return true;
}

WOLFSSL_CTX* nothingness::network::SSL::Context::getWolfSSLContext(){
	return ctx;
}

nothingness::network::SSL::Context::~Context(){
	wolfSSL_CTX_free(ctx);
}
