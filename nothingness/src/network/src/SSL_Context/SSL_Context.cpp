#include "SSL_Context.h"
#include <wolfssl/ssl.h>
#include "..//..//network.h"

nothingness::network::SSL::Context::Context() {

}

bool nothingness::network::SSL::Context::use_certificate_file(const char* ceritficate_file, uint16_t ssl_file_type){
	if (wolfSSL_CTX_use_certificate_file(ctx, ceritficate_file, ssl_file_type) != SSL_SUCCESS) {
		nothingness::network::last_error = "failed to load: \"";
		nothingness::network::last_error += ceritficate_file;
		nothingness::network::last_error += "\", please check the file.";
		
		nothingness::network::last_error_code = 54;
		return false;
	}
	
	return true;
}

bool nothingness::network::SSL::Context::use_PrivateKey_file(const char* PrivateKey_file, uint16_t ssl_file_type){
	if (wolfSSL_CTX_use_PrivateKey_file(ctx, PrivateKey_file, ssl_file_type) != SSL_SUCCESS) {
		nothingness::network::last_error = "failed to load: \"";
		nothingness::network::last_error += PrivateKey_file;
		nothingness::network::last_error += "\", please check the file.";

		nothingness::network::last_error_code = 55;
		return false;
	}

	return true;
}

bool nothingness::network::SSL::Context::load_verify_locations(const char* certificate_file, const char* _file){
	int ret = wolfSSL_CTX_load_verify_locations(ctx, certificate_file, _file);
	if (ret != SSL_SUCCESS) {
		nothingness::network::last_error = "failed to load: \"";
		nothingness::network::last_error += certificate_file;
		nothingness::network::last_error += "\", please check the file.";
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
