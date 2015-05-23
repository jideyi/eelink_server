/*
 * sql.h
 *
 *  Created on: May 20, 2015
 *      Author: jk
 */

#ifndef SRC_SQL_H_
#define SRC_SQL_H_

int mysql_initial();
int mysql_add(void* arg);
void mysql_quit();

#endif /* SRC_SQL_H_ */
