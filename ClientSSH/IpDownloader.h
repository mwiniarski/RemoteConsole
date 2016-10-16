#include <thread>
#include <string>
#include <Windows.h>
#include <curl/curl.h>
using namespace std;

/*
*	Get my public ip from api site.
*	Check for dropbox file.
*	Change its name to my IP address.
*/
//callback needed for getting curl's return value
size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

class IpDownloader{
private:
	string ip;
public:

	IpDownloader(){
		//curl variables
		CURL *curl;

		curl_global_init(CURL_GLOBAL_ALL);
		//list of headers
		struct curl_slist *chunk = NULL;
		string readBuffer;

		//everything OK 
		if (curl = curl_easy_init()) {

			//init curl stuff
			curl_easy_setopt(curl, CURLOPT_POST, true);
			curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
			curl_easy_setopt(curl, CURLOPT_NOBODY, 0);
			//callback func
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

			//add header info
			chunk = curl_slist_append(chunk, "Authorization: Bearer 4LB8CnShD0gAAAAAAAAJcLKjzBg-ggTgoIUOuvFT3wBFJLlPXz4dRO21fZs_IgQf");
			chunk = curl_slist_append(chunk, "Content-Type: application/json");
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);

			//part1 - check for files on dropbox app
			string data = "{"
				"\"path\": \"\","
				"\"recursive\": false,"
				"\"include_media_info\": false,"
				"\"include_deleted\": false,"
				"\"include_has_explicit_shared_members\": false"
				"}";
			char * url = "https://api.dropboxapi.com/2/files/list_folder";

			//perform request
			curl_easy_setopt(curl, CURLOPT_URL, url);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
			curl_easy_perform(curl);

			//readBuffer - return data from list folder request
			readBuffer = readBuffer.substr(39, 16);
			ip = readBuffer.substr(0, readBuffer.find_first_of("\""));
		}
	}
	
	const char* get()
	{
		return ip.c_str();
	}
};