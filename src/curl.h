/*
 * curl.h
 *
 *  Created on: Apr 30, 2015
 *      Author: jk
 */

#ifndef SRC_CURL_H_
#define SRC_CURL_H_

#include <curl/curl.h>

CURL* initCurlHandleOfLeancloud();
CURL* initCurlHandleOfYeelink();


#endif /* SRC_CURL_H_ */
