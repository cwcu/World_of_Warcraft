#include <iostream>
#include <vector>
#include <string>
#include <map>
using namespace std;

int dragonInitHP, ninjaInitHP, icemanInitHP, lionInitHP, wolfInitHP;
int dragonInitATK, ninjaInitATK, icemanInitATK, lionInitATK, wolfInitATK;
int N;

class Warrior {
public:
    int ID;
    int HP;
    int ATK;
    int faction; // 0 : red, 1 : blue
    // int city; // 0 : red headquarter, 1 - N, N + 1 : blue Headquarter
    int reward;
    int take;
    int death;
    int steps;
    int kills;
    string Type;

    Warrior() {}
    Warrior(int faction_, int ID_) {
        ID = ID_;
        faction = faction_;
        // city = faction == 0? 0 : N + 1; 
        reward = 0;
        take = 0;
        death = 0;
        steps = 0;
        kills = 0;
    }

    // 1 : win, 0 : tie, -1 : loss
    virtual int fight(Warrior* e, string& Time, int& cityM, int& city) {
        cout << Time << ' ' << (faction == 0? "red" : "blue") << ' ' << Type << ' ' << ID 
        << " attacked " << (e->faction == 0? "red" : "blue") << ' ' << e->Type << ' ' << e->ID
        << " in city " << city << " with " << HP << " elements and force " << ATK << endl;
        
        int ret;
        int prev = e->HP;

        if ((e->HP -= ATK) <= 0) {
            if (e->Type == "lion") HP += prev;
            e->dead(Time, city);
            
            reward = 1;
            take = cityM;
            ret = 1;
        }
        else {
            if (e->fightBack(this, Time, cityM, city)) ret = -1;
            else ret = 0;
        }

        return ret;
    }

    // 1 : win, 0 : tie
    virtual int fightBack(Warrior* e, string& Time, int& cityM, int& city) {
        cout << Time << ' ' << (faction == 0? "red" : "blue") << ' ' << Type << ' ' << ID
        << " fought back against " << (e->faction == 0? "red" : "blue") << ' ' << e->Type << ' '
        << e->ID << " in city " << city << endl;
        
        int prev = e->HP;

        if ((e->HP -= ATK / 2) <= 0) {
            if (e->Type == "lion") HP += prev;
            e->dead(Time, city);
  
            reward = 1;    
            take = cityM;     
            return 1;
        }
        
        return 0;
    }

    virtual void dead(string& Time, int& city) {
        death = 1;
        cout << Time << ' ' << (faction == 0? "red" : "blue") << ' ' << Type << ' ' << ID << " was killed in city " << city << endl;
    }
};

class Dragon : public Warrior {
public:
    Dragon() {}
    Dragon(int faction_, int ID_) : Warrior(faction_, ID_) {
        HP = dragonInitHP;
        ATK = dragonInitATK;
        Type = "dragon";    
    }

    int fight(Warrior* e, string& Time, int& cityM, int& city) {
        int ret = Warrior::fight(e, Time, cityM, city);

        if (ret >= 0) cheer(Time, city);
        return ret;
    } 

    void cheer(string& Time, int& city) {
        cout << Time << ' ' << (faction == 0? "red" : "blue") << ' ' << Type << ' ' << ID << " yelled in city " << city << endl;
    }
};

class Ninja : public Warrior {
public: 
    Ninja() {}
    Ninja(int faction_, int ID_) : Warrior(faction_, ID_) {
        HP = ninjaInitHP;  
        ATK = ninjaInitATK;  
        Type = "ninja";      
    }

    int fightBack(Warrior* e, string& Time, int& cityM, int& city) {
        return 0;
    }
    
};

class Iceman : public Warrior {
public:  
    Iceman() {}
    Iceman(int faction_, int ID_) : Warrior(faction_, ID_) {
        HP = icemanInitHP;  
        ATK = icemanInitATK;      
        Type = "iceman";  
    }
};

class Lion : public Warrior {
public:   
    Lion() {}
    Lion(int faction_, int ID_) : Warrior(faction_, ID_) {
        HP = lionInitHP;  
        ATK = lionInitATK;  
        Type = "lion";      
    } 
};

class Wolf : public Warrior {
public:    
    Wolf() {}
    Wolf(int faction_, int ID_) : Warrior(faction_, ID_) {
        HP = wolfInitHP;  
        ATK = wolfInitATK;      
        Type = "wolf";  
    }
    
    int fight(Warrior* e, string& Time, int& cityM, int& city) {
        cout << Time << ' ' << (faction == 0? "red" : "blue") << ' ' << Type << ' ' << ID 
        << " attacked " << (e->faction == 0? "red" : "blue") << ' ' << e->Type << ' ' << e->ID
        << " in city " << city << " with " << HP << " elements and force " << ATK << endl;
        
        int ret;
        int prev = e->HP;

        if ((e->HP -= ATK) <= 0) {
            kills++;
            if (kills % 2 == 0) {
                HP *= 2;
                ATK *= 2;
            }
            
            if (e->Type == "lion") HP += prev;
            e->dead(Time, city);
            
            reward = 1;
            take = cityM;
            ret = 1;
        }
        else {
            if (e->fightBack(this, Time, cityM, city)) ret = -1;
            else ret = 0;
        }

        return ret;
    }
};

class Headquarter{
public:
    int M;
    map<int, Warrior*> w;
    int k;
    vector<string> cur;
    int enemy;
    int F;

    Headquarter() {
        w = map<int, Warrior*>();
        k = 1;
        cur = vector<string>();
        enemy = 0;
    }

    ~Headquarter() {
        while (w.size() > 0) {
            auto it = w.begin();
            delete it->second;
            w.erase(it);
        }
    }

    virtual void eraseDeaths() {
        vector<int> toErase;
        for (auto&& p : w) if (p.second->death == 1) toErase.push_back(p.second->ID);
        for (auto && i : toErase) {
            delete w[i];
            w.erase(i);
        }
    }

    virtual bool produce(int faction, string& Time) {
        bool success = false;
        string Type = cur[(k - 1) % 5];
        if (Type == "dragon") {
            if (M >= dragonInitHP) {
                success = true;
                M -= dragonInitHP;
                // w[k] = Dragon(faction, k); virtual function fight() can't be used like this.
                w[k] = new Dragon(faction, k);
                k++;
            }
        }
        else if (Type == "ninja") {
            if (M >= ninjaInitHP) {
                success = true;
                M -= ninjaInitHP;
                w[k] = new Ninja(faction, k);
                k++;
            }           
        }
        else if (Type == "iceman") {
             if (M >= icemanInitHP) {
                success = true;
                M -= icemanInitHP;
                w[k] = new Iceman(faction, k);
                k++;
            }             
        }
        else if (Type == "lion") {
            if (M >= lionInitHP) {
                success = true;
                M -= lionInitHP;
                w[k] = new Lion(faction, k);
                k++;
            }                    
        }
        else if (Type == "wolf") {
            if (M >= wolfInitHP) {
                success = true;
                M -= wolfInitHP;
                w[k] = new Wolf(faction, k);
                k++;
            }              
        }

        if (success)
            cout << Time << ' ' << (F == 0? "red" : "blue") << ' ' << w[k - 1]->Type << ' ' << k - 1 << " born" << endl;     
        return success;
    }    

    virtual void report(string& Time) {
        cout << Time << ' ' << M << " elements in " << (F == 0? "red" : "blue") << " headquarter" << endl;
    }

    virtual void get() {
        for (auto&& p : w) {
            M += p.second->take;
            p.second->take = 0;
        }
    }

    virtual bool taken(string& Time) {
        if (enemy >= 2) {
            cout << Time << ' ' << (F == 0? "red" : "blue") << " headquarter was taken" << endl;
            return true;
        }
        else return false;
    }
};

class Red : public Headquarter{
public:
    Red() {}
    Red(int M_) {
        Headquarter::M = M_;
        Headquarter::cur = {"iceman", "lion", "wolf", "ninja", "dragon"};
        F = 0;
    }
};

class Blue : public Headquarter{
public:
    Blue() {}
    Blue(int M_) {
        Headquarter::M = M_;
        Headquarter::cur = {"lion", "dragon", "ninja", "iceman", "wolf"};
        F = 1;
    }  
};

Headquarter* r;
Headquarter* b;

class City{
public:
    int ID;
    int M;
    int red;
    int lateRed;
    int blue;
    int lateBlue;
    int flag; // -1 : no flag, 0 : red, 1 : blue
    int last; // 1 : red win last time, -1 : blue, 0 : tie

    City() {
        M = 0;
        red = -1;
        lateRed = -1;
        blue = -1;
        lateBlue = -1;
        flag = -1;
        last = 0;
    }

    void clearReward() {
        if (red != -1) r->w[red]->reward = 0;
        if (blue != -1) b->w[blue]->reward = 0;
    }

    void sendR() {
        if (r->M < 8) return;
        if (red != -1 && r->w[red]->reward == 1) {
            r->w[red]->HP += 8;
            r->M -= 8;
            r->w[red]->reward = 0;
        }
    }

    void sendB() {
        if (b->M < 8) return;
        if (blue != -1 && b->w[blue]->reward == 1) {
            b->w[blue]->HP += 8;
            b->M -= 8;
            b->w[blue]->reward = 0;
        }        
    }

    void eraseDeaths() {
        if (red != -1 && r->w.find(red) == r->w.end()) red = -1;
        if (blue != -1 && b->w.find(blue) == b->w.end()) blue = -1;
    }

    void taken(string& Time) {
        if (red == -1 && blue != -1) {
            b->M += M;
            
            cout << Time << ' ' << "blue" << ' ' << b->w[blue]->Type << ' ' << b->w[blue]->ID 
            << " earned " << M << " elements for his headquarter" << endl;       
             
            M = 0;   
        }
        else if (red != -1 && blue == -1) {
            r->M += M;
            
            cout << Time << ' ' << "red" << ' ' << r->w[red]->Type << ' ' << r->w[red]->ID 
            << " earned " << M << " elements for his headquarter" << endl;       
             
            M = 0;              
        }
    }

    void increment() {
        M += 10;
    }

    void icemanMarch(Warrior* w) {
        if (w->steps == 1) {
            w->steps = 0;
            w->ATK += 20;
            if ((w->HP -= 9) <= 0) w->HP = 1;
        }
        else w->steps++;  
    }
    
    void marchOut(string& Time, City& left) {
        if (blue == -1) return;

        if (b->w[blue]->Type == "iceman") icemanMarch(b->w[blue]);

        if (ID == 1) {
            cout << Time << ' ' << "blue" << ' ' << b->w[blue]->Type << ' ' << b->w[blue]->ID << ' '
        << "reached red headquarter with " << b->w[blue]->HP << " elements and force " << b->w[blue]->ATK << endl;
            lateBlue = max(lateBlue, -1);
            r->enemy++;
        }
        else {
            cout << Time << ' ' << "blue" << ' ' << b->w[blue]->Type << ' ' << b->w[blue]->ID << ' '
            << "marched to city " << ID - 1 << " with " << b->w[blue]->HP << " elements and force " << b->w[blue]->ATK << endl;
            left.lateBlue = max(left.lateBlue, blue);
            lateBlue = max(lateBlue, -1);
        }
    }

    void marchIn(string& Time, City& left) {
        if (left.ID != ID && left.red == -1) return;

        if (left.ID == ID) {
            if (r->w[r->k - 1]->Type == "iceman") icemanMarch(r->w[r->k - 1]);
            cout << Time << ' ' << "red" << ' ' << r->w[r->k - 1]->Type << ' ' << r->w[r->k - 1]->ID << ' '
            << "marched to city " << ID << " with " << r->w[r->k - 1]->HP << " elements and force " << r->w[r->k - 1]->ATK << endl;   
            lateRed = max(lateRed, r->k - 1);
            return;         
        }

        if (r->w[left.red]->Type == "iceman") icemanMarch(r->w[left.red]);

        cout << Time << ' ' << "red" << ' ' << r->w[left.red]->Type << ' ' << r->w[left.red]->ID << ' '
        << "marched to city " << ID << " with " << r->w[left.red]->HP << " elements and force " << r->w[left.red]->ATK << endl;
        lateRed = max(lateRed, left.red);
        left.lateRed = max(left.lateRed, -1);      
    }
    
    void rightMarchIn(string& Time) {
        if (b->w[b->k - 1]->Type == "iceman") icemanMarch(b->w[b->k - 1]);
        
        cout << Time << ' ' << "blue" << ' ' << b->w[b->k - 1]->Type << ' ' << b->w[b->k - 1]->ID << ' '
        << "marched to city " << ID << " with " << b->w[b->k - 1]->HP << " elements and force " << b->w[b->k - 1]->ATK << endl;
        lateBlue = max(lateBlue, b->k - 1);
    }

    void rightMarchOut(string& Time) {
        if (red == -1) return;

        if (r->w[red]->Type == "iceman") icemanMarch(r->w[red]);

        cout << Time << ' ' << "red" << ' ' << r->w[red]->Type << ' ' << r->w[red]->ID << ' '
        << "reached blue headquarter with " << r->w[red]->HP << " elements and force " << r->w[red]->ATK << endl;
        lateRed = max(lateRed, -1);
        b->enemy++;       
    }

    void march() {
        red = lateRed;
        lateRed = -1;
        blue = lateBlue;
        lateBlue = -1;
    }

    void war(string& Time) {
        if (red != -1 && blue != -1) {
            int res;
            Warrior* w1 = r->w[red];
            Warrior* w2 = b->w[blue];
            
            if ((flag == -1 && ID % 2 == 1)
            || flag == 0) {
                res = w1->fight(w2, Time, M, ID);
            }
            else {
                res = -w2->fight(w1, Time, M, ID);
            }
            
            if (res == 1) {
                cout << Time << ' ' << "red" << ' ' << w1->Type << ' ' << w1->ID 
                << " earned " << M << " elements for his headquarter" << endl;
            }
            else if (res == -1) {
                cout << Time << ' ' << "blue" << ' ' << w2->Type << ' ' << w2->ID 
                << " earned " << M << " elements for his headquarter" << endl;               
            }

            if (res != 0) M = 0;

            if (res + last == 2 && flag != 0) {
                cout << Time << " red flag raised in city " << ID << endl;
                flag = 0;
            }
            else if (res + last == -2 && flag != 1) {
                cout << Time << " blue flag raised in city " << ID << endl;
                flag = 1;
            }
            
            last = res;
        }
    }
};

// curT mins -> hhh:mm
inline string parseTime(int hour, int min) {
    string H = to_string(hour);
    while (H.size() < 3) H = '0' + H;
    string M = to_string(min);
    while (M.size() < 2) M = '0' + M;

    return H + ':' + M;
}

int main() {
    int numOfCases;
    scanf("%d", &numOfCases);
    
    for (int caseNo = 1; caseNo <= numOfCases; ++caseNo) {
        int M, T;
        scanf("%d%d%d", &M, &N, &T);
        scanf("%d%d%d%d%d", &dragonInitHP, &ninjaInitHP, &icemanInitHP, &lionInitHP, &wolfInitHP);
        scanf("%d%d%d%d%d", &dragonInitATK, &ninjaInitATK, &icemanInitATK, &lionInitATK, &wolfInitATK);

        printf("Case:%d\n", caseNo);

        // Liskov Substitution principle
        r = new Red(M);
        b = new Blue(M);
        vector<City> cities(N + 1); // cities[1,...,N]
        for (int i = 1; i <= N; ++i) cities[i].ID = i;

        bool End = false;
        for (int h = 0; h * 60 <= T && !End; ++h) {
            for (int m = 0; m < 60 && h * 60 + m <= T; m += 10) {
                string Time = parseTime(h, m);
                bool rProduce, bProduce;
                if (m == 0) {
                    rProduce = r->produce(0, Time);
                    bProduce = b->produce(1, Time);
                }
                else if (m == 10) {
                    for (int i = 1; i <= N; ++i) {
                        cities[i].marchOut(Time, cities[i - 1]);
                        if (i == 1) End |= r->taken(Time);

                        if (i > 1) cities[i].marchIn(Time, cities[i - 1]);
                        else if (rProduce) cities[i].marchIn(Time, cities[i]);
                    }

                    if (bProduce) cities[N].rightMarchIn(Time);
                    cities[N].rightMarchOut(Time);
                    End |= b->taken(Time);

                    for (int i = 1; i <= N; ++i) cities[i].march();

                    if (End) break;
                }
                else if (m == 20) {
                    for (int i = 1; i <= N; ++i) cities[i].increment();
                }
                else if (m == 30) {
                    for (int i = 1; i <= N; ++i) cities[i].taken(Time);
                }
                else if (m == 40) {
                    for (int i = 1; i <= N; ++i) {
                        cities[i].war(Time);
                    }
                    
                    r->eraseDeaths();
                    b->eraseDeaths();
                    for (int i = 1; i <= N; ++i) cities[i].eraseDeaths();

                    // after sending HP to warrior according to this priority rule (the closet warriors to the enemy Headquarter),  
                    // we must set Warrior::reward = 0 for those warriors that aren't sent HP, so they will not be rewarded mistakenly in the next hour
                    for (int i = N; i >= 1; --i) cities[i].sendR();
                    r->get();
                    
                    for (int i = 1; i <= N; ++i) cities[i].sendB();
                    b->get();

                    for (int i = 1; i <= N; ++i) cities[i].clearReward();
                }
                else if (m == 50) {
                    r->report(Time);
                    b->report(Time);
                }
            }
        }

        delete r;
        delete b;
    }
    
    return 0;
}