#include <iostream>
#include <vector>
#include <map>
#include <string>


using namespace std;

struct node
{
    int n_used = 0;
    bool is_terminal = false;
    vector<node *> links = vector<node *>(26, nullptr);
};



void add_str(const string& s,  node * const root) {
    node * cur = root;
    for (int pos = 0; pos < s.size(); ++pos) {
        if (cur->links[s[pos] - 'a'] == nullptr) {
            cur->links[s[pos] - 'a'] = new node;
            
        }
        cur = cur->links[s[pos] - 'a'];
    }
    cur->is_terminal = true;

}



bool find_str(const string& s,const node* const root) {
    const node* cur = root;

    for (auto simb : s) {
        if (cur->links[simb - 'a']) {
            cur = cur->links[simb - 'a'];
        }
        else {
            return false;
        }
    }

    return cur->is_terminal;
}



void find_str(const string& s, node* const root) {
    node* cur = root;

    for (auto simb : s) {
        if (cur->links[simb - 'a']) {
            cur = cur->links[simb - 'a'];
        }
        else {
            return;
        }
    }

    cur->is_terminal = false;

}

/// <summary>
/// Напомнить что так делать плохо, пусть подумают почему?
/// ответ копирование строки кажый вызов - очень плохо
/// </summary>
/// <param name="root"></param>
/// <param name="s"></param>
void show_tree(const node* root, string s = "") {
    if (root->is_terminal) {
        cout << s << endl;
    }
    for (int i = 0; i < 26; ++i) {
        if (root->links[i]) {
            string add = "a";
            add[0] += i;
            show_tree(root->links[i], s + add);
        }
    }
}



int main()
{
    int n = 0;
    cin >> n;
    node* root = new node();
    for (int i = 0; i < n; ++i) {
        string s;
        cin >> s;
        add_str(s, root);
    }

    show_tree(root);
}

