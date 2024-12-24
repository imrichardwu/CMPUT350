#include "Histo.h"

int main(){
    Histo histo;          
    std::string word;

    // Read words from standard input until EOF
    while(std::cin >> word){
        histo.count(word);  // Count each word
    }

    histo.print(std::cout); // Print the histogram to standard output

    return 0; 
}
