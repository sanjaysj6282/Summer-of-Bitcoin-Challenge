#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <climits>
#include <map>

using namespace std;

#define ll long long

#define vi vector<ll> 
#define vs vector<string> 
#define pi pair<int, int>
#define ff first
#define ss second
#define vii vector<pi>
#define vvi vector<vi> 
#define rep(i,a,b) for(ll i=a;i<b;i++)

const ll M=5500, MOD=1e9+7;


// We cant use a greedy approach 'cause when we sort it we change the order and its difficult 
// to check "A transaction may only appear in a block if all of its parents appear earlier in the block"

// So we  check each one so we use DP like either we have to take the fee or not take it
// This problem is modified version of 0-1 Knapsack

// Map to say if parents apperared earlier the block
// and Also display if its included or not
ll dp[M][M];
map<string, int> mp;
ll solve(vs Text_Id, vi Fee, vi Weight, vs Parents, ll W, ll n){
    if(n <= 0 || W <= 0)
        return 0;

    if(dp[n][W] != -1)
        return dp[n][W];

    if(Weight[n-1] > W){
        // Doesn't include nth item
        mp[Text_Id[n-1]]=0;
        dp[n][W] = solve(Text_Id, Fee, Weight, Parents, W, n-1);
    }

    else{
        bool k=solve(Text_Id, Fee, Weight, Parents, W, n-1) < 
              (solve(Text_Id, Fee, Weight, Parents, W-Weight[n-1], n-1) + Fee[n-1]);
        // Wheather its parent exist before it OR parent is not present
        if(k && (mp[Parents[n-1]]==1 || Parents[n-1].length()<=10)){
            // Include nth item
            mp[Text_Id[n-1]]=1;
            dp[n][W]= solve(Text_Id, Fee, Weight, Parents, W-Weight[n-1], n-1) + Fee[n-1];
        }
        else{
            // Doesn't include nth item
            mp[Text_Id[n-1]]=0;
            dp[n][W]= solve(Text_Id, Fee, Weight, Parents, W, n-1);
        }   
    }  

    return dp[n][W];
}

int main(){

    ifstream ip("mempool.csv");

    if(!ip.is_open()) 
        cout << "ERROR: File Open" << '\n';

    string text_id;
    string fee;
    string weight;
    string parents;

    vs Text_Id;
    vi Fee;
    vi Weight;
    vs Parents;

    // To prevent 1st row not to be added to the array
    getline(ip,text_id,',');
    getline(ip,fee,',');
    getline(ip,weight,',');
    getline(ip,parents,'\n');

    while(ip.good()){

        getline(ip,text_id,',');
        getline(ip,fee,',');
        getline(ip,weight,',');
        getline(ip,parents,'\n');

        Text_Id.push_back(text_id);
        mp[text_id]=0;
        Fee.push_back(stoi(fee));
        Weight.push_back(stoi(weight));
        Parents.push_back(parents);
    }

    rep(i, 0, M)
        rep(j, 0, M)
            dp[i][j]=-1;

    ll n=Fee.size();
    ll ans = solve(Text_Id, Fee, Weight, Parents, 4000000, n);


    ofstream fout;

    string fname="block.txt";
    // To open a file and deleting the contents inside
    fout.open(fname, ofstream::out | ofstream::trunc);
 
    for(ll i=0; i<n; i++)
        if(mp[Text_Id[i]]==1)
            fout << Text_Id[i] << '\n';


    fout.close();
    ip.close();

}

// References
// https://github.com/tpatil2/C-Programs/blob/master/RWcsv/rwcsv.cpp
// https://www.codezclub.com/cpp-store-enter-data-file-using-file-handling/
// https://stackoverflow.com/questions/17032970/clear-data-inside-text-file-in-c
