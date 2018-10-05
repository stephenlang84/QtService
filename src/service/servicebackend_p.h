#ifndef QTSERVICE_SERVICEBACKEND_P_H
#define QTSERVICE_SERVICEBACKEND_P_H

#include "servicebackend.h"

namespace QtService {

class ServiceBackendPrivate
{
public:
	ServiceBackendPrivate(Service *service);

	Service *service;
	bool operating = false;
};

}

#endif // QTSERVICE_SERVICEBACKEND_P_H
