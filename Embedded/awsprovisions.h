#ifndef AWS_PROVISIONS_H
#define AWS_PROVISIONS_H

// AWS IoT Core settings
const char *awsEndpoint = ""; //Your AWS IoT endpoint, usually found in Settings of the IoT console
const int port = 8883; //MQTT port for secure connections

// Device Certificate
const char *deviceCert = R"EOF(
-----BEGIN CERTIFICATE-----
-----END CERTIFICATE-----
)EOF";

// Private Key
const char *privateKey = R"EOF(
-----BEGIN RSA PRIVATE KEY-----
-----END RSA PRIVATE KEY-----
)EOF";

// Root CA
const char *rootCA = R"EOF(
-----BEGIN CERTIFICATE-----
-----END CERTIFICATE-----
)EOF";

#endif // AWS_PROVISIONS_H
