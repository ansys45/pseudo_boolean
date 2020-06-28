#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <list>
#include <cmath>
#include <ctime>
#include <unordered_set>
 

using namespace std;





//вывод вектора 
template <typename T>
ostream& operator << (ostream& out, const vector<T>&  v) 
{
    for (int i = 0; i < v.size(); i++) out << v[i] << " ";
    return out;
}

ostream& operator << (ostream& out, const unordered_set<int>&  s) 
{
    unordered_set<int>::iterator it;
    for (it = s.begin(); it != s.end(); it++) out << *it << "  ";
    return out;
}

struct node 
{
    unordered_set<int> term;
    long double coef = 0;
    long double csum = 0; // сумма коэфициантов детей
    vector<node*> parents;
    vector<node*> children;
};


struct node* newNode(long double coef, unordered_set<int> term) 
{
    struct node* node = new struct node();
    if (*term.begin() != 0)
        term.insert(0);
    node->term = term;
    node->coef = coef;
    return(node); 
}


ostream& operator << (ostream& out, const node* nd) 
{
    out << "( " << nd->coef << "   " << nd->term << ")";
    return out;
}

bool operator == (const node& nd1, const node& nd2) 
{
    return (nd1.term == nd2.term);
}


// subset
bool operator < (const node& nd1, const node& nd2) 
{
    for (auto it : nd1.term) {
        if (nd2.term.find(it) == nd2.term.end()){
            return 0;
        }
    }
    return 1;
}


void addCoefs(vector<vector<node*> >& func, node* nd)
{   
    // cout << nd << endl;
    for (auto it : func[1]) {
        if (*it < *nd)
            it->csum += nd->coef;
    }
    func[0][0]->csum += nd->coef;
}


void connectNodes(node* nd1, node* nd2) 
{   
    if (*nd1 == *nd2)
        return;
    
    if (*nd1 < *nd2) {
        nd1->children.push_back(nd2);
        nd2->parents.push_back(nd1);

    } else {
        nd2->children.push_back(nd1);
        nd1->parents.push_back(nd2);
    } 
}


void addNode(vector<vector<node*> >& func, node* nd) 
{
    int lvl = nd->term.size() - 1;
    for (auto it : func[lvl]) {
        if (*it == *nd) {
            it->coef += nd->coef;
            return;
        }
    }
    func[lvl].push_back(nd);

}


void connectFunc(vector<vector<node*> >& func)
{
    int lvl = func.size() - 1;
    while (!func[lvl].size()) lvl--;

    bool seen;
    while (lvl) {
        // vector<node*>::iterator it = func[lvl].begin();
        for (auto nd : func[lvl]) {
            seen = 0;
            for (auto it : func[lvl - 1]) {
                if (*it < *nd) {
                    connectNodes(nd, it);
                    seen = 1;
                }
            }
            if (!seen) {
                func[lvl - 1].push_back(nd);
                func[lvl].erase(find(func[lvl].begin(), func[lvl].end(), nd));
            } else {
                addCoefs(func, nd);
            }
        }
        lvl--;
    }
}

vector<vector<node*> > FUNC;
long double BEST = 1000000000;
vector<int> SOLUTION;
int B = 0;

long double substituteSolution2(vector<int>& solution)
{   
    node* nd = FUNC[0][0];
    long double res = nd->coef;

    node* s = new node;
    unordered_set<node*> visited;
    list<node*> queue;
    queue.push_back(nd);

    while (!queue.empty()) {

        s = queue.front();
        queue.pop_front();

        for (auto c : s->children) {
            if (visited.find(c) == visited.end()) {
                visited.insert(c);

                bool has_zero = false;
                for (auto x : c->term) {
                    if (!solution[x]) {
                        has_zero = true;
                        break;
                    }
                }

                if (has_zero)
                    continue;
                
                res += c->coef;
                queue.push_back(c);
            }
        }
    }

    return res;
}


int element(vector< vector<long double> >& v, int low, int high) 
{ 
    long double pivot = v[high][0];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (v[j][0] <= pivot) { 
            i++; 
            swap(v[i], v[j]); 
        } 
    } 
    swap(v[i + 1], v[high]); 
    return (i + 1); 
} 

void QuickSort(vector< vector<long double> >& v, int low, int high) 
{ 
    if (low < high) {
        int pi = element(v, low, high);
        QuickSort(v, low, pi - 1); 
        QuickSort(v, pi + 1, high); 
    } 
}


vector<vector<node*> > makeFunc(string file)
{
    ifstream fin;
    fin.open(file);

    int L, C, i, j;
    long double trash, fc, tc;

    fin >> L >> C;

    vector<vector<node*> > func;
    func.resize(L+1);

    // fc
    func[0].push_back(newNode(0, {0}));
    for (i = 1; i <= L; i++) {
        fin >> trash >> fc;

        func[0][0]->coef += fc;
        addNode(func, newNode(-fc, {i}));
    }   

    // tc
    for (i = 1; i <= C; i++) {
        fin >> trash;

        vector<vector<long double> >  column;
        for (j = 1; j <= L; j++) {
            vector<long double> loc; // from j location to i client
			fin >> tc;
			loc.push_back(tc);
			loc.push_back(j);
			column.push_back(loc);
        }

        QuickSort(column, 0, L - 1);

        func[0][0]->coef += column[0][0];
        unordered_set<int> trm;
        trm.insert(column[0][1]);
        for (j = 1; j < L; j++) {
            node* nd = newNode(column[j][0] - column[j - 1][0], trm);
            addNode(func, nd);
            trm.insert(column[j][1]);
        }
    }

    return func;
}


int nodeNum(node* nd)
{
    for (auto x : nd->term)
        if (x) return x;
    return 0;
}


void branch(vector<int> solution) 
{   
    B++;

    if (!count(solution.begin(), solution.end(), 2)) {
        long double s = substituteSolution2(solution);
        if (s < BEST) {
            BEST = s;
            SOLUTION = solution;
        }
        return;
    }

    vector<long double> coefs(FUNC[1].size() + 1, 0);
    vector<long double> violations = coefs;
    unordered_set<node*> visited;

    long double maxV = 0;
    int maxV_x = 1;
    long double minC = 10000000;
    int minC_x = 1;

    long double minC_2 = 1000000;

    long double LB = FUNC[0][0]->coef;
    vector<vector<long double> > lb;

    
    for (auto nd : FUNC[1]) {
        int x_num = nodeNum(nd);
        long double x_coef = nd->coef;


        if (!solution[x_num])
            continue;
        

        if (x_coef >= 0) {
            solution[x_num] = 0;
            return branch(solution);
        }
        LB += x_coef;

        list<node*> queue;
        queue.push_back(nd);
        node* s = new node;

        if (solution[x_num] == 1) {
            while (!queue.empty()) {
                s = queue.front();
                queue.pop_front();

                for (auto c : s->children) {
                    if (visited.find(c) == visited.end()) {

                        bool has_zero = false;
                        bool has_two = false;

                        for (auto x : c->term) {
                            if (!solution[x]) {
                                has_zero = true;
                                break;
                            }

                            if (solution[x] == 2) {
                                has_two = true;
                                break;
                            }
                        }

                        if (has_two)
                            continue;

                        visited.insert(c);

                        if (has_zero)
                            continue;

                        LB += c->coef;
                        queue.push_back(c);
                    }
                }
            }
            // continue;
        }

        else {
            while (!queue.empty()) {
                s = queue.front();
                queue.pop_front();

                for (auto c : s->children) {
                    if (visited.find(c) == visited.end()) {
                        visited.insert(c);
                        bool has_zero = false;
                        bool has_two = false;

                        vector<int> tempV;
                        tempV.push_back(x_num);

                        for (auto x : c->term) {
                            if (!solution[x]) {
                                has_zero = true;
                                break;
                            }

                            if (x != x_num && solution[x] == 2) {
                                has_two = true;
                                tempV.push_back(x);
                            }
                        }

                        if (has_zero)
                            continue;

                        if (!has_two) {
                            x_coef += c->coef;

                            if (x_coef >= 0) {
                                solution[x_num] = 0;
                                return branch(solution);
                            }

                            // LB += c->coef;
                        }

                        vector<long double> l;
                        l.push_back(c->coef);

                        if (minC_2 > c->coef)
                            minC_2 = c->coef;

                        for (int i = 0; i < tempV.size(); i++) {
                            violations[tempV[i]] += c->coef;
                            l.push_back(tempV[i]);
                        }

                        lb.push_back(l);
                        queue.push_back(c);
                    }
                }
            }
            
            // LB += x_coef;

            coefs[x_num] = x_coef;
            violations[x_num] += x_coef;

            if (-maxV > x_coef) {
                maxV = -x_coef;
                maxV_x = x_num;
            }

            if (-minC < x_coef) {
                minC = -x_coef;
                minC_x = x_num;
            }
        }
    }
    
    for (int i = 0; i < lb.size(); i++) {
        bool good = true;
        for (int j = 1; j < lb[i].size(); j++) {
            if (coefs[lb[i][j]] + lb[i][0] >= 0) {
                good = false;
                break;
            }
        }

        if (good) {
            for (int j = 1; j < lb[i].size(); j++) 
                coefs[lb[i][j]] += lb[i][0];
                    
            LB += lb[i][0];
        }
    }

    if (LB >= BEST)
        return;

    for (int i = 1; i < violations.size(); i++) {
        if (violations[i] < 0) {
            solution[i] = 1;
            return branch(solution);
        }

        if (violations[i] > maxV) {
            maxV = violations[i];
            maxV_x = i;
        }
    }
  
    solution[maxV_x] = 1;
    branch(solution);

    solution[maxV_x] = 0;
    branch(solution);
}


int main()
{   
    vector<string> tests = {"capEx1.txt", "cap71.txt", "cap72.txt", "cap73.txt", "cap74.txt", "cap101.txt", "cap102.txt", "cap103.txt", 
                            "cap104.txt", "cap131.txt", "cap132.txt", "cap133.txt", "cap134.txt"};
    
    vector<int> answers = {36, 932615, 977799, 1010641, 1034976, 796648, 854704, 893782, 928941, 793439, 851495, 893076, 928941};

    clock_t beginT = clock();

    int cnt = 0;
    for (int i = 0; i < tests.size(); i++) {
        cout << endl << "Test: " << tests[i] << endl;
        clock_t begin = clock();

        FUNC = makeFunc(tests[i]);
        connectFunc(FUNC);

        vector<int> sol(FUNC[1].size() + 1, 2);
        sol[0] = 1;
        
        BEST = 100000000;
        B = 0;
        branch(sol);


        cout << (int)BEST << endl;
        // cout << SOLUTION << endl;
        cout << "B = " << B << endl;

        if ((int)BEST != answers[i]) {
            cout << "INCORRECT\t" << answers[i] << endl;
            cnt++;
        }

        cout << (double)(clock() - begin) / CLOCKS_PER_SEC << endl;
        // break;
    }

    cout << endl << "Total time: " << (double)(clock() - beginT) / CLOCKS_PER_SEC << endl;
    cout << "Incorrect answers: " << cnt << endl;
    return 0;
}