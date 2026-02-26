#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <curl/curl.h>

using namespace std;

// Function to store downloaded HTML
size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* data) {
    data->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main() {

    CURL* curl;
    CURLcode res;
    string htmlContent;

    curl = curl_easy_init();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://books.toscrape.com/");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &htmlContent);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    // Open CSV file
    ofstream file("books_data.csv");
    file << "Name,Price,Rating\n";

    // Regex patterns
    regex titlePattern("title=\"([^\"]+)\"");
    regex pricePattern("<p class=\"price_color\">([^<]+)</p>");
    regex ratingPattern("star-rating ([A-Za-z]+)");

    sregex_iterator titleIter(htmlContent.begin(), htmlContent.end(), titlePattern);
    sregex_iterator priceIter(htmlContent.begin(), htmlContent.end(), pricePattern);
    sregex_iterator ratingIter(htmlContent.begin(), htmlContent.end(), ratingPattern);
    sregex_iterator end;

    while (titleIter != end && priceIter != end && ratingIter != end) {

        string title = (*titleIter)[1];
        string price = (*priceIter)[1];
        string rating = (*ratingIter)[1];

        file << "\"" << title << "\"," << price << "," << rating << "\n";

        ++titleIter;
        ++priceIter;
        ++ratingIter;
    }

    file.close();

    cout << "✅ Data scraped and saved to books_data.csv" << endl;

    return 0;
}