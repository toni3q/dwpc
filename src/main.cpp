#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<random>
#include<curl/curl.h>
#include"funcs.h"

#define MAX_ATTEMPTS 3

// we need this function in order to convert our CURLcode variable into a readeable string
size_t translate_for_human(void* contents, size_t size, size_t nmemb, std::string* userp) {
    size_t totalSize = size * nmemb;
    userp->append((char*)contents, totalSize);
    return totalSize;
}

// we use this function to save the downloaded content into a file
void save_on_file(std::string content){
    std::ofstream file;
    file.open("./src/downloads/" + random_id(8) + ".html");
    if(file.is_open()){
        file.clear();
        file << content;
        file.close();
    } else {
        std::cerr<<"Couldn't save downloaded content";
    }
}

int main(){
    CURL* handle = curl_easy_init();
    std::string html_buffer;                                                    
    
    /* Setting up curl based on our needs. */
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, translate_for_human);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &html_buffer);
    curl_easy_setopt(handle, CURLOPT_URL, "https://news.ycombinator.com");
    curl_easy_setopt(handle, CURLOPT_TIMEOUT_MS, 500);
    CURLcode result;

    int performed_attempts = 0;
    do{
        result = curl_easy_perform(handle);
        if(result == CURLE_OK){
            save_on_file(html_buffer);
            std::cout << "Content saved successfully: " << curl_easy_strerror(result) << std::endl;
            break;
        } else {
            std::cerr << "Couldn't perform any download: " << curl_easy_strerror(result) << std::endl;
            performed_attempts++;
            curl_easy_setopt(handle, CURLOPT_TIMEOUT_MS, 5000+(performed_attempts*5000));
        }
    }while(performed_attempts < 3);

    curl_easy_cleanup(handle);
    return 0;
}