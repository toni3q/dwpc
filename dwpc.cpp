#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<random>
#include<curl/curl.h>

std::string random_id(int lenght){
    static std::random_device rd;                                                                   //hardware seed
    static std::mt19937 gen(rd());                                                                  //motore mersenne twister
    std::uniform_int_distribution<> distrib(0, 9);                                                  //init rand
    std::stringstream ss;                                                                           //init ss

    int arr_identifier[lenght];
    for(int pos = 0; pos < lenght; pos++){
        ss << distrib(gen);                                                                         //send data to ss
    }

    return ss.str();
}

// we need this function in order to convert our CURLcode variable into a readeable string
size_t translate_for_human(void* contents, size_t size, size_t nmemb, std::string* userp) {
    size_t totalSize = size * nmemb;
    userp->append((char*)contents, totalSize);
    return totalSize;
}

// we use this function to save the downloaded content into a file
void save_on_file(std::string content){
    std::ofstream file;
    file.open("downloads/" + random_id(8) + ".html");
    if(file.is_open()){
        file.clear();
        file << content;
        file.close();
    } else {
        std::cerr<<"Couldn't save downloaded content";
    }
}

int main(){
    CURL* handle = curl_easy_init();                                                                // initializing download handle and html_buffer
    std::string html_buffer;                                                    
    
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, translate_for_human);                           // without translate_for_human() you can't get a clear readable page
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &html_buffer);
    curl_easy_setopt(handle, CURLOPT_URL, "https://news.ycombinator.com");                          // setting up curl and providing an URL to the function
    curl_easy_setopt(handle, CURLOPT_TIMEOUT_MS, 5000);                                             // setting up a timeout time in milliseconds

    CURLcode result = curl_easy_perform(handle);                                                    // variable type CURLcode to store the result


    if(result != CURLE_OK){
        std::cerr<<"There is something wrong: "<<curl_easy_strerror(result)<<std::endl;
    } else {
        save_on_file(html_buffer);                                                                  // saving the readeable content into a file
        std::cout<<"Content downloaded successfully: "<<curl_easy_strerror(result)<<std::endl;
    }

    curl_easy_cleanup(handle);
    return 0;
}