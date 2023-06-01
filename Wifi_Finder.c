#include <stdio.h>
#include <windows.h>
#include <wlanapi.h>
#pragma comment(lib, "wlanapi.lib")

void error_handling(const char* const msg, ...) {
	va_list args;
	va_start(args, msg);
	vfprintf(stderr, msg, args);
	fputc('\n', stderr);
	va_end(args);
	exit(1);
}

int main(void) {
	DWORD dwResult = 0;
	HANDLE hClient = NULL;
	PWLAN_INTERFACE_INFO_LIST wlanInfoList = NULL;
	PWLAN_AVAILABLE_NETWORK_LIST wlanNetworkList = NULL;

	if (WlanOpenHandle(WLAN_API_VERSION, NULL, &dwResult, &hClient) != ERROR_SUCCESS) error_handling("WlanOpenHandle failed, error : %d\n", dwResult);

	if ((dwResult = WlanEnumInterfaces(hClient, NULL, &wlanInfoList)) != ERROR_SUCCESS) {
		WlanCloseHandle(hClient, NULL);
		error_handling("WlanEnumInterfaces failed, error : %d\n", dwResult);
	}

	if ((dwResult = WlanGetAvailableNetworkList(hClient, &(wlanInfoList->InterfaceInfo[0].InterfaceGuid), 0, NULL, &wlanNetworkList)) != ERROR_SUCCESS) {
		WlanFreeMemory(wlanInfoList);
		WlanCloseHandle(hClient, NULL);
		error_handling("WlanGetAvailableNetworkList failed, error: %d\n", dwResult);
	}

	puts("Detected Wi-Fi Networks :\n");
	for (DWORD i = 0; i < wlanNetworkList->dwNumberOfItems; i++) {
		printf("SSID : %s\n", wlanNetworkList->Network[i].dot11Ssid.ucSSID);
	}

	WlanFreeMemory(wlanNetworkList);
	WlanFreeMemory(wlanInfoList);
	WlanCloseHandle(hClient, NULL);
	
	return 0;
}
