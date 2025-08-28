#include<iostream>
#include<vector>

using namespace std;

const int INF = 1e9;

struct node {
    int l, r;
    int max_val;
    int push_val = 0;
    node * left_son = nullptr;
    node * right_son = nullptr;
};


node * build_tree(const vector<int> & v, int l, int r) {
    node * new_node = new node;
    new_node->l = l;
    new_node->r = r;
    if (r - l == 1) {
        new_node->max_val = v[l];
        return new_node;
    }

    int m = (l + r) / 2;
    new_node->left_son = build_tree(v, l, m);
    new_node->right_son = build_tree(v, m, r);
    new_node->max_val = max(new_node->left_son->max_val, new_node->right_son->max_val);
    return new_node;
}

void build_tree_vec(const vector<int> & v, vector<int> & seg_tree,int l, int  r, int i) {
    if (r - l == 1) {
        seg_tree[i] = v[l];
        return;
    }

    int m = (l + r) / 2;
    build_tree_vec(v, seg_tree, l, m, 2 * i + 1);
    build_tree_vec(v, seg_tree, m, r, 2 * i + 2);
    seg_tree[i] = max(seg_tree[2 * i + 1], seg_tree[2 * i + 2]);
}   


int get_max(const node * root, int l, int r) {
    if (!root) {
        cerr << "node dont exist\n" ;
        return -INF;
    }

    if (r <= root->l || l >= root->r) {
        return -INF;
    } else if (r >= root->r && l <= root->l) {
        return root->max_val;
    } else {
        return max(get_max(root->left_son, l, r), get_max(root->right_son, l, r));
    }

}


int  get_max_vec(const vector<int> & seg_tree, int l_cur, int r_cur, int l_q, int r_q, int i = 0) {
    if (r_q <= l_cur || l_q >= r_cur) {
        return -INF;
    } else if(r_q >= r_cur && l_q <= l_cur) {
        return seg_tree[i];
    }
    int  m = (l_cur + r_cur) / 2;
    return max(get_max_vec(seg_tree, l_cur, m, l_q, r_q, 2 * i + 1),
                get_max_vec(seg_tree, m, r_cur, l_q, r_q, 2 * i + 2));
}


void   change_one_elem_vec(vector<int> & seg_tree, int l_cur, int r_cur, int pos, int new_val, int i = 0) {
    if (pos < l_cur || pos >= r_cur) {
        return;
    }
    if (r_cur - l_cur == 1) {
        seg_tree[i] = new_val;
    } else  {
        int m = (l_cur + r_cur) / 2;
        change_one_elem_vec(seg_tree, l_cur, m, pos, new_val, i * 2 + 1);
        change_one_elem_vec(seg_tree, m, r_cur, pos, new_val, i * 2 + 2);
        seg_tree[i] = max(seg_tree[2 * i + 1], seg_tree[2 * i + 2]);
    }
}


ostream & operator<< (ostream & out, const vector<int> & v) {
    for (auto el : v) {
        cout << el << ' ';
    }
    return out;
}


void   change_seg_vec(vector<int> & seg_tree, vector<int> & push_seg_tree, int l_cur, int r_cur, int pos_l, int pos_r, int add_val, int i = 0) {  
    if (pos_r <= l_cur || pos_l >= r_cur) {
        return;
    }
    if (pos_r >= r_cur && pos_l <= l_cur) {
        push_seg_tree[i] += add_val;
    } else  {
        int m = (l_cur + r_cur) / 2;
        change_seg_vec(seg_tree, push_seg_tree, l_cur, m, pos_l, pos_r, add_val, i * 2 + 1);
        change_seg_vec(seg_tree, push_seg_tree, m, r_cur, pos_l, pos_r, add_val, i * 2 + 2);
        seg_tree[i] = max(seg_tree[2 * i + 1] + push_seg_tree[2 * i + 1], 
                          seg_tree[2 * i + 2] + push_seg_tree[2 * i + 2]);
    }
}


void relax(vector<int> & seg_tree, vector<int> & push_seg_tree, int  l, int r, int i) {
    if (push_seg_tree[i] == 0) {
        return;
    }

    seg_tree[i] += push_seg_tree[i];
    if  (r - l > 1) {
        push_seg_tree[2 * i + 1] += push_seg_tree[i];
        push_seg_tree[2 * i + 2] += push_seg_tree[i];
    }

    push_seg_tree[i] = 0;
}


int  get_max_vec_upd(vector<int> & seg_tree, vector<int> & push_seg_tree, int l_cur, int r_cur, int l_q, int r_q, int i = 0) {
    relax(seg_tree, push_seg_tree, l_cur, r_cur, i);
    if (r_q <= l_cur || l_q >= r_cur) {
        return -INF;
    } else if(r_q >= r_cur && l_q <= l_cur) {
        return seg_tree[i];
    }
    int  m = (l_cur + r_cur) / 2;
    return max(get_max_vec_upd(seg_tree, push_seg_tree, l_cur, m, l_q, r_q, 2 * i + 1),
                get_max_vec_upd(seg_tree, push_seg_tree, m, r_cur, l_q, r_q, 2 * i + 2));
}




int main() {
    node * root;
    vector<int> v = {1, 3, 4, 3, 1, 7};
    vector<int> seg_tree(15);
    vector<int> seg_tree_push(15);
    root = build_tree(v, 0, v.size());
    
    build_tree_vec(v, seg_tree, 0, v.size(),  0);
    cout << seg_tree << endl;

    ///change_one_elem_vec(seg_tree, 0, v.size(), 4, 10);
    change_seg_vec(seg_tree, seg_tree_push, 0, v.size(), 1, 5, 10);
    cout << seg_tree << endl;
    cout << seg_tree_push << endl;

    cout << '\n';
    int m;
    cin >> m;
    for (int i = 0; i < m; ++i) {
        int l, r;
        cin >> l >> r;
        cout << get_max_vec_upd(seg_tree, seg_tree_push, 0, v.size(), l, r) << '\n';
        cout << seg_tree << endl;
        cout << seg_tree_push << endl;
    }
}