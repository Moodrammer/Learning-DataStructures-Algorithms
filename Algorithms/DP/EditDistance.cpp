// Reference : https://www.youtube.com/watch?v=MiqoA-yF-0M&t=518s (is a really Great video :D)

#include "iostream"
#include "vector"
#include "algorithm"
using namespace std;
typedef long long ll;

ll editDistance(string a, string b);

int main() {
    cout << "Calculating the edit distance between two strings" << endl;
    string a , b;
    cout << "Enter string a : \n";
    cin >> a;
    cout << "Enter string b: \n";
    cin >> b;
    cout << editDistance(a, b) << endl; 
}

/*
    The case for each cell is as follows if we consider the last two characters only in the current
    two substrings, we have three operations.
    For Example : if we want to turn string "mahm" to string "abcd"
    we have three options:
    Option [1]: Replacement
      convert "mah" -> "abc" then replace the 'm' with 'd'
    Option [2]: Deletion
      Remove the 'm' from "mahm" -> "mah"
      Convert "mah" -> "abcd"
    Option [3]: Insertion
      Convert "mahm" -> "abc"
      Insert 'd' at the end
    --------- -----------
    | replace | Insert  |
    --------- -----------
    | Delete  | current |
    --------- -----------
    The tree options as shown in the table
  ---------------------------------------------------------------------
      m  a  h  m
    0 1  2  3  4
  a 1       |  |
  b 2       |  |
  c 3 ---- [1][3]
  d 4 ---- [2] .
  ---------------------------------------------------------------------
  - We take the minimum edit distance in the previous three operations and add 1 if the current two
  characters don't match , because if the current two characters match then this won't change the edit
  distance as we need to do nothing
*/
ll editDistance(string a, string b) {
    ll lengthA = a.size() + 1;
    ll lengthB = b.size() + 1;
    // Define a table (multi dimensional array) with the size of lengthA * lengthB
    vector<vector<ll>> table(lengthB, vector<ll>(lengthA, 0));
    // for each column the editing distance between the current prefix c and the empty string
    // corresponds to c deletions
    for(int c = 0; c < lengthA; c++) table[0][c] = c;
    // for each row the editing distance between the current string of length r and the empty string
    // corresponds to r insertions
    for(int r = 0; r < lengthB; r++) table[r][0]  = r;
    for(int r = 1; r < lengthB; r++) {
        for(int c = 1; c < lengthA; c++) {
            if(a[c - 1] == b[r - 1]) {
                table[r][c] = min(table[r-1][c-1], min(table[r-1] [c], table[r][c-1]));
            }
            else {
                table[r][c] = 1 + min(table[r-1][c-1], min(table[r-1] [c], table[r][c-1]));
            }
        }
    }
    return table[lengthB - 1][lengthA - 1];
}