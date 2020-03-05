#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <random>
#include <climits>
#include <cmath>
#include <ctime>
#include <time.h>

#define inf INT_MAX

using namespace std;

template <typename T>
ostream& operator << (ostream& out, const vector<T>&  v) {
    for (long double i = 0; i < v.size(); i++) out << v[i] << "  ";
    return out;
}


template <typename T>
ostream& operator << (ostream& out, const set<T>&  s) {
    set<int>::iterator i = s.begin();
    for (; i != s.end(); i++) out << *i << " ";
    return out;
}


struct Comp {
    bool operator()(const set<int>& l, const set<int>& r) const {
    	if (l.size() > r.size())
    		return 0;
    	if (l.size() == r.size()) {
    		if (*l.begin() > *r.begin())
    			return 0;
    	}
    	return 1;
    }
};


int element(vector< vector<long double> >& v, int low, int high) { 
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

void QuickSort(vector< vector<long double> >& v, int low, int high) { 
    if (low < high) {
        int pi = element(v, low, high);
        QuickSort(v, low, pi - 1); 
        QuickSort(v, pi + 1, high); 
    } 
}


bool changed = 0;
void K1(map<set<int>, pair<long double, long double>, Comp>& func, vector<int>& loc) {

	map<set<int>, pair<long double, long double>, Comp>::iterator it;
	map<set<int>, pair<long double, long double>, Comp>::iterator it2;
	map<set<int>, pair<long double, long double>, Comp>::iterator it3;
	vector<int> k1;
	vector<int> k2;
	vector<set<int> > add;
	vector<long double> add_co;
	bool z;
	int i;
	set<int> emp;
    emp.insert(-1);

	if (func.size() == 1)
		return;

    for(it = ++func.begin(); it != func.end(); ++it) {
		z = 0;
    	
    	if ((it->first).size() == 1 && (it->second).first >= 0) {
    		k1.push_back(*(it->first).begin());

    		loc[*(it->first).begin()] = 1;	// 1 if open
    	}

		if ((it->first).size() == 1 && (it->second).first + (it->second).second < 0) {
			k2.push_back(*(it->first).begin());
			
			loc[*(it->first).begin()] = 2;	// 2 if closed

			add.push_back(emp);
			add_co.push_back((it->second).first);

			it = func.erase(it);
			--it;
		}
    
    	for (int i = 0; i < k1.size(); i++) {
    		if ((it->first).find(k1[i]) != (it->first).end()) {
    			if ((it->first).size() > 1) {
    				set<int>::iterator sit;
    				long double c = (it->second).first;
    				for (sit = (it->first).begin(); sit != (it->first).end(); ++sit) {
						it3 = func.begin();
    					if (*sit != k1[i]) {
    						set<int> C;
    						C.insert(*sit);
    						for (it2 = it3; it2 != func.end(); ++it2) {
								++it3;
    							if (it2->first == C) break;
    						}
    						it2->second.second -= c;
    					}
    				}
    			}
    			it = func.erase(it);
				--it;
    			changed = 1;
				z = 1;
    			break;
    		}
    	}

		if (!z) {
			if ((it->first).size() > 1) {
				long double own_c = (it->second).first;
				set<int>::iterator var;

				set<int> new_term;
				for (var = (it->first).begin(); var != (it->first).end(); ++var) {
					vector<int>::iterator search = find(k2.begin(), k2.end(), *var);
					if (search == k2.end()) new_term.insert(*var);
					else changed = 1;
				}
				if (new_term.size() < (it->first).size()) {
					if (new_term.size() == 0) {
						add.push_back(emp);
						add_co.push_back(own_c);
						
					}
					else {
						add.push_back(new_term);
						add_co.push_back(own_c);
					}

					it = func.erase(it);
						--it;
				}
				
			}
		}
		
    }

	for (i = 0; i < add.size(); i++) {
    	for (it = func.begin(); it != func.end(); ++it) {
			if (it->first == add[i]) {
				(it->second).first += add_co[i];
				if (add[i].size() == 1 && it->first != emp)
					(it->second).second -= add_co[i];
				break;
			}
		}
		if (it == func.end())
			func[add[i]].first = add_co[i];
    }
	
}

/* ТЕПЕРЬ КУМАВАЛЫ ОБЪЕДИНЕНЫ !
void K2(map<set<int>, pair<long double, long double>, Comp>& func, vector<int>& loc) {

	int i;
	set<int> emp;
    emp.insert(-1);
	vector<int> k2;
	vector<set<int> > add;
	vector<long double> add_co;
	map<set<int>, pair<long double, long double>, Comp>::iterator it;

	if (func.size() == 1)
		return;


	for(it = ++func.begin(); it != func.end(); ++it) {

		if ((it->first).size() == 1 && (it->second).first + (it->second).second < 0) {
			k2.push_back(*(it->first).begin());
			
			loc[*(it->first).begin()] = 2;	// 2 if closed

			add.push_back(emp);
			add_co.push_back((it->second).first);

			it = func.erase(it);
			--it;
		}

		if ((it->first).size() > 1) {
			long double own_c = (it->second).first;
			set<int>::iterator var;

			set<int> new_term;
			for (var = (it->first).begin(); var != (it->first).end(); ++var) {
				vector<int>::iterator search = find(k2.begin(), k2.end(), *var);
				if (search == k2.end()) new_term.insert(*var);
				else changed = 1;
			}
			if (new_term.size() < (it->first).size()) {
				if (new_term.size() == 0) {
					add.push_back(emp);
					add_co.push_back(own_c);
					
				}
				else {
					add.push_back(new_term);
					add_co.push_back(own_c);
				}

				it = func.erase(it);
					--it;
			}
		 	
		}


	}
   
    for (i = 0; i < add.size(); i++) {
    	for (it = func.begin(); it != func.end(); ++it) {
			if (it->first == add[i]) {
				(it->second).first += add_co[i];
				if (add[i].size() == 1 && it->first != emp)
					(it->second).second -= add_co[i];
				break;
			}
		}
		if (it == func.end())
			func[add[i]].first = add_co[i];
    }
	
}
*/

void Relax(map<set<int>, pair<long double, long double>, Comp>& func, vector<int>& loc) {

	K1(func, loc);

	vector<int>::iterator search = find(loc.begin(), loc.end(), 0);

	if (!changed || search == loc.end())
		return;

	changed = 0;
	Relax(func, loc);
}


void Substitute(map<set<int>, pair<long double, long double>, Comp>& func, vector<int>& loc, int val, int place) {
	
	map<set<int>, pair<long double, long double>, Comp>::iterator it;
	map<set<int>, pair<long double, long double>, Comp>::iterator it3;
	int i;

	if (val == 1) {
		loc[place] = 1;
		map<set<int>, pair<long double, long double>, Comp>::iterator it2;

	    for(it = ++func.begin(); it != func.end(); ++it) {
	    
    		if ((it->first).find(place) != (it->first).end()) {
    			if ((it->first).size() > 1) {
    				set<int>::iterator var;
    				long double own_c = (it->second).first;
    				for (var = (it->first).begin(); var != (it->first).end(); ++var) {
						it3 = func.begin();
						if (*var != place) {
    						set<int> trm;
    						trm.insert(*var);
    						for (it2 = it3; it2 != func.end(); ++it2) {
								++it3;
    							if (it2->first == trm) break;
    						}
    						it2->second.second -= own_c;
    					}
    				}
    			}

    			it = func.erase(it);
				--it;
    			changed = 1;
    		}
	    }

	}

	else {
		loc[place] = 2;	// 2 if closed
		set<int> emp;
	    emp.insert(-1);
		vector<set<int> > add;
		vector<long double> add_co;

		for(it = ++func.begin(); it != func.end(); ++it) {

			if ((it->first).size() == 1 && place == *(it->first).begin()) {
				
				add.push_back(emp);
				add_co.push_back((it->second).first);

				it = func.erase(it);
				--it;
			}

			if ((it->first).size() > 1) {
				long double own_c = (it->second).first;
				set<int>::iterator var;

				set<int> new_term;
				for (var = (it->first).begin(); var != (it->first).end(); ++var) {
					if (*var != place) new_term.insert(*var);
					else changed = 1;
				}
				if (new_term.size() < (it->first).size()) {
					if (new_term.size() == 0) {
						add.push_back(emp);
						add_co.push_back(own_c);
						
					}
					else {
						add.push_back(new_term);
						add_co.push_back(own_c);
					}

					it = func.erase(it);
					--it;
				}
			 	
			}


		}

		it3 = func.begin();
	    for (i = 0; i < add.size(); i++) {
	    	for (it = it3; it != func.end(); ++it) {
				if (it->first == add[i]) {
					(it->second).first += add_co[i];
					if (add[i].size() == 1 && it->first != emp)
						(it->second).second -= add_co[i];
					break;
				}
			}
			if (it == func.end()) {
				++it3;
				func[add[i]].first = add_co[i];
			}
	    }
	}
}


long double firstE(map<set<int>, pair<long double, long double>, Comp>& Ham) {
	if (Ham.size() == 1) return -1;
	map<set<int>, pair<long double, long double>, Comp>::iterator it;
	it = ++Ham.begin();
	return *(it->first.begin());
}

long double opt = inf;
vector<int> opt_loc;


long double lower_bound(map<set<int>, pair<long double, long double>, Comp>& Ham) {
	if (Ham.size() == 1) return inf;
	map<set<int>, pair<long double, long double>, Comp>::iterator it = Ham.begin();

	long double res = it->second.first;
	while (it->first.size() == 1) {
		res += it->second.first;
		++it;
	}

	map<set<int>, pair<long double, long double>, Comp>::iterator it2 = Ham.begin();
	while (it2 != Ham.end()) {
		cout << it2->first << "\t" << it2->second.first << endl;
		++it2;
	}
	cout << res << endl << endl;

	return res;
}


void solve(map<set<int>, pair<long double, long double>, Comp>& res, vector<int>& loc) {
	
	if (res.size() == 1) {
		// cout << (res.begin()->second).first << endl;
		opt = min(opt, ((res.begin()->second).first));
		opt_loc = loc;
		return;
	}

	// if (lower_bound(res) >= opt) return;

	long double pos = firstE(res);

	Relax(res, loc);

	map<set<int>, pair<long double, long double>, Comp> new_res = res;
	Substitute(new_res, loc, 1, pos);
	Substitute(res, loc, 2, pos);
	solve(new_res, loc);
	solve(res, loc);
	
}


int L, C;
int main() {
	clock_t time = clock();
	ifstream fin;
	fin.open("cap72.txt");

	fin >> L >> C;
	map<set<int>, pair<long double, long double> > H;
	set<int> emp; // free term
	emp.insert(-1);
	H[emp].first = 0;
	// Saving Fixed Costs
	long double trash;
	long double fc;
	for (int i = 0; i < L; i++) {
		fin >> trash;
		fin >> fc;


		// Creating the Hammer Function
		// 1.from FC
		H[emp].first += fc;
		set<int> cur;
		cur.insert(i);
		H[cur].first -= fc;
	}

	// Creating the Hammer Function = Filling terms and coef
	// 2. from TC
	long double tc;
	for (int i = 0; i < C; i++) {
		fin >> trash;
		vector< vector<long double> > column;
		for (int j = 0; j < L; j++) {
			vector<long double> loc; // from j location to i client
			fin >> tc;
			loc.push_back(tc);
			loc.push_back(j);
			column.push_back(loc);	// column is created to order the elements for Hammer
		}
		// sorting the column
		QuickSort(column, 0, L - 1);

		set<int> term;
		H[emp].first += column[0][0];
		term.insert(column[0][1]);
		for (int l = 1; l < L; l++) {
			long double own_coef = column[l][0] - column[l - 1][0];
			H[term].first += own_coef;

			if (term.size() > 1) {
				set<int>::iterator set_it;
				for (set_it = term.begin(); set_it != term.end(); set_it++) {
					set<int> temp;
					temp.insert(*set_it);
					H[temp].second += own_coef;
				}
			}

			term.insert(column[l][1]);
		}

	}
	

	map<set<int>, pair<long double, long double>, Comp> Hammer;
	map<set<int>, pair<long double, long double> >::iterator it = H.begin();
	for(; it != H.end(); it++) {
		pair<long double, long double> temp;
		temp.first = it->second.first;
		temp.second = it->second.second;
		Hammer[it->first] = temp;
	}
	vector<int> loc;
	for (int i = 0; i < L; i++) loc.push_back(0);	// vector loc is the main vector of locations
													// 0 = untouched; 1 = open; 2 = closed

	/////////////////////////////// SOLVING //////////////////////////////////////////////
	
	solve(Hammer, loc);

	cout << opt << endl;
	cout << opt_loc << endl;

	cout << endl << (double)(clock() - time) / CLOCKS_PER_SEC << endl;
}


// можно улучшить, если СОВМЕСТИТЬ КУМАВАЛЫ !!!!!!!!!













































