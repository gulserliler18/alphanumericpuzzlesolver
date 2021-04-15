/*Egemen Gülserliler
150180103
06/04/2021*/

#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <chrono> //c++11 standart is required for compilation
#include <fstream>

using namespace std;
struct node
{
    char x;
    int val;
    node *parent;
    vector<node *> children;
    node()
    {
        for (int i = 0; i < 10; i++)
        {
            available.emplace_back(i);
            parent = NULL;
        }
    };
    node(char x, int val, vector<int> thus, node *parent)
    {
        this->x = x;
        this->val = val;
        for (int i = 0; i < thus.size(); i++)
        {
            available.emplace_back(thus[i]);
        }
        this->parent = parent;
    };
    vector<int> available;
    void print() { cout << x << ", " << val << endl; };
};

typedef struct node node;
typedef struct char_num char_num;

void create_set(string operation, vector<char> &set)
{
    for (int i = 0; i < operation.length(); i++)
    {
        if (i == 0)
        {
            set.emplace_back(operation[0]);
        }
        bool exists = false;
        for (int j = 0; j < set.size(); j++)
        {
            if (operation[i] == set[j])
            {
                exists = true;
                break;
            }
        }
        if (!exists)
        {
            set.emplace_back(operation[i]);
        }
    }
}

template <typename T>
void remove_from_set(T c, vector<T> &set)
{
    for (int i = 0; i < set.size(); i++)
    {
        if (c == set[i])
        {
            set.erase(set.begin() + i);
            break;
        }
    }
}

template <typename T>
int search_set(T c, vector<T> set)
{
    for (int i = 0; i < set.size(); i++)
    {
        if (set[i] == c)
            return i;
    }
    return -1;
}

struct tree
{
    node *root;
    string operand1, operand2, result;
    vector<char> set;
    void create_tree(node *, int);
    tree(vector<char> set, string &operand1, string &operand2, string &result)
    {
        root = new node;
        this->set = set;
        this->operand1 = operand1;
        this->operand2 = operand2;
        this->result = result;
        create_tree(root, 0);
    };
    void DFS(vector<node *> &);
    void BFS(vector<node *> &);
    void print_all(node *this_one)
    {
        this_one->print();
        cout << endl;
        for (int i = 0; i < this_one->children.size(); i++)
        {
            print_all(this_one->children[i]);
        }
    };
};

typedef struct tree tree;

void tree::create_tree(node *traverse, int welche)
{
    if (welche == set.size())
    {
        return;
    }

    for (int j = 0; j < traverse->available.size(); j++)
    {
        if (((set[welche] == operand1[0]) || (set[welche] == operand2[0]) || (set[welche] == result[0])) && (traverse->available[j] == 0))
        {
            continue;
        }

        if ((operand1.length() < result.length()) && (operand2.length() < result.length()) && (traverse->available[j] != 1) && (set[welche] == result[0]))
        {
            continue;
        }

        traverse->children.emplace_back(new node(set[welche], traverse->available[j], traverse->available, traverse));
        remove_from_set(traverse->children[traverse->children.size() - 1]->val, traverse->children[traverse->children.size() - 1]->available);
        create_tree(traverse->children[traverse->children.size() - 1], welche + 1);
    }
};

void tree::DFS(vector<node *> &sol)
{
    vector<node *> visited, stck;
    int max_size = 0;
    visited.emplace_back(root);
    for (int i = root->children.size() - 1; i > -1; i--)
    {
        stck.emplace(stck.begin(), root->children[i]);
    }
    while (stck.size())
    {
        visited.emplace_back(stck[0]);
        int where = -1;
        for (int j = 0; j < sol.size(); j++)
        {
            if (stck[0]->x == sol[j]->x)
            {
                where = j;
                break;
            }
        }
        if (where != -1)
        {
            sol[where] = stck[0];
        }
        else
        {
            sol.emplace_back(stck[0]);
        }
        if (stck[0]->x == set[set.size() - 1])
        {
            int op1 = 0, op2 = 0, res = 0, mult;
            char tk;
            for (int i = operand1.length() - 1; i > -1; i--)
            {
                tk = operand1[(operand1.length() - 1) - i];
                for (int j = 0; j < sol.size(); j++)
                {
                    if (tk == sol[j]->x)
                    {
                        mult = sol[j]->val;
                    }
                }
                op1 += pow(10, i) * mult;
            }
            for (int i = operand2.length() - 1; i > -1; i--)
            {
                tk = operand2[(operand2.length() - 1) - i];
                for (int j = 0; j < sol.size(); j++)
                {
                    if (tk == sol[j]->x)
                    {
                        mult = sol[j]->val;
                    }
                }
                op2 += pow(10, i) * mult;
            }
            for (int i = result.length() - 1; i > -1; i--)
            {
                tk = result[(result.length() - 1) - i];
                for (int j = 0; j < sol.size(); j++)
                {
                    if (tk == sol[j]->x)
                    {
                        mult = sol[j]->val;
                    }
                }
                res += pow(10, i) * mult;
            }
            if ((op1 + op2) == res)
            {
                cout << "Algorithm: DFS" << endl
                     << "Number of the Visited Nodes: " << visited.size() << endl
                     << "Maximum number of nodes kept in the memory: " << max_size << endl;
                for (int i = 0; i < sol.size(); i++)
                {
                    cout << sol[i]->x << " = " << sol[i]->val << ", ";
                }
                cout << endl;
                return;
            }
        }
        if (stck.size() > max_size)
            max_size = stck.size();
        stck.erase(stck.begin());
        for (int i = visited[visited.size() - 1]->children.size() - 1; i > -1; i--)
        {
            stck.emplace(stck.begin(), visited[visited.size() - 1]->children[i]);
        }
    }
}

void tree::BFS(vector<node *> &sol)
{
    vector<node *> visited, queue;
    node *finder;
    int max_size = 0;
    visited.emplace_back(root);
    for (int i = root->children.size() - 1; i > -1; i--)
    {
        queue.emplace(queue.begin(), root->children[i]);
    }
    while (queue.size())
    {
        visited.emplace_back(queue[0]);
        if (queue[0]->x == set[set.size() - 1])
        {
            int op1 = 0, op2 = 0, res = 0, mult;
            char tk;
            for (int i = operand1.length() - 1; i > -1; i--)
            {
                finder = queue[0];
                tk = operand1[(operand1.length() - 1) - i];
                while (finder != root)
                {
                    if (tk == finder->x)
                    {
                        mult = finder->val;
                        break;
                    }
                    finder = finder->parent;
                }
                op1 += pow(10, i) * mult;
            }
            for (int i = operand2.length() - 1; i > -1; i--)
            {
                finder = queue[0];
                tk = operand2[(operand2.length() - 1) - i];
                while (finder != root)
                {
                    if (tk == finder->x)
                    {
                        mult = finder->val;
                        break;
                    }
                    finder = finder->parent;
                }
                op2 += pow(10, i) * mult;
            }
            for (int i = result.length() - 1; i > -1; i--)
            {
                finder = queue[0];
                tk = result[(result.length() - 1) - i];
                while (finder != root)
                {
                    if (tk == finder->x)
                    {
                        mult = finder->val;
                        break;
                    }
                    finder = finder->parent;
                }
                res += pow(10, i) * mult;
            }
            if ((op1 + op2) == res)
            {
                cout << "Algorithm: BFS" << endl
                     << "Number of the Visited Nodes: " << visited.size() << endl
                     << "Maximum number of nodes kept in the memory: " << max_size << endl;
                finder = queue[0];
                while (finder != root)
                {
                    cout << finder->x << " = " << finder->val << ", ";
                    sol.emplace(sol.begin(), finder);
                    finder = finder->parent;
                }
                cout << endl;
                return;
            }
        }
        if (queue.size() > max_size)
            max_size = queue.size();
        queue.erase(queue.begin());
        for (int i = 0; i < visited[visited.size() - 1]->children.size(); i++)
        {
            queue.emplace_back(visited[visited.size() - 1]->children[i]);
        }
    }
}

int main(int argc, char *argv[])
{
    auto start = chrono::high_resolution_clock::now();
    vector<node *> sol;
    vector<char> set;
    if ((argc < 6) || (argc > 6))
    {
        cout << "Missing or extra parameters!";
        return 0;
    }

    string operand1, operand2, result, operation, method;
    method = argv[1];
    operand1 = argv[2];
    operand2 = argv[3];
    result = argv[4];

    operation = operand1 + operand2 + result;
    create_set(operation, set);
    tree *hlt = new tree(set, operand1, operand2, result);

    if (method == "DFS")
        hlt->DFS(sol);
    else
        hlt->BFS(sol);
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    auto total_duration = duration.count();
    cout << "runtime: " << (double)total_duration / 1000000 << "s";
    ofstream of;
    of.open(argv[5]);
    for (int i = -1; i < 10; i++)
    {
        if (i >= 0)
        {
            of << i;
        }
        if (i != 9)
            of << '\t';
    }
    of << endl;
    for (int i = 0; i < sol.size(); i++)
    {
        int j;
        of << sol[i]->x << '\t';
        for (j = 0; j < 10; j++)
        {
            if (j == sol[i]->val)
                of << "1\t";
            else
                of << ".\t";
        }
        long pos = of.tellp();
        of.seekp(pos - 1);
        of << endl;
    }
    return 0;
}