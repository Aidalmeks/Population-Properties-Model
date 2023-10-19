#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <cstdlib> // for rand()
#include <cmath> // for pow()

class Person {
public:
    Person(const std::string& lastName, int age, double wealth)
        : lastName_(lastName), age_(age), wealth_(wealth), talent_("beta"), mutationDuration_(0) {}
    Person* same;

    std::string getLastName() const { return lastName_; }
    void setLastName(const std::string& lastName) { lastName_ = lastName; }
    void ageOneYear() {
        ++age_;
        if (mutationDuration_ > 0) {
            --mutationDuration_;
            if (mutationDuration_ == 0) {
                talent_ = "beta";  // 10年后突变有效期结束，talent回归beta
            }
        }
        checkMutation();
        applyTalentEffect();
    }

    int getAge() const { return age_; }
    double getWealth() const { return wealth_; }
    void modifyWealth(double amount) { wealth_ += amount; }

private:
    std::string lastName_;
    int age_;
    double wealth_;
    std::string talent_; // 默认为beta
    int mutationDuration_;  // 添加突变持续时间变量

private:

    void checkMutation() {
        double mutationChance = static_cast<double>(rand() % 1000) / 10.0;
        if (mutationChance < 0.1) {
            talent_ = "alpha";
        }
        else if (mutationChance >= 0.1 && mutationChance < 33.43) {
            talent_ = "gamma";
        } // 如果mutationChance >= 33.43，则不进行任何操作
        if (mutationChance < 33.43) {
            mutationDuration_ = 10;  // 设置突变的有效期为10年
        }
    }


    void applyTalentEffect() {
        if (talent_ == "alpha") {
            // 指数型增长
            wealth_ *= 1.2; // 示例:每年增长20%
        }
        else if (talent_ == "beta") {
            // 线性增长
            wealth_ += 100*age_; // 示例:每年增长与年龄相当的数目
        }
        else if (talent_ == "gamma") {
            // 衰减
            wealth_ *= 0.95; // 示例:每年衰减5%
        }
    }
};

class Man;
class Woman;

class Marriage {
public:
    Marriage(Man* husband, Woman* wife) : husband_(husband), wife_(wife) {}
    Man* getHusband() const { return husband_; }
    Woman* getWife() const { return wife_; }

private:
    Man* husband_;
    Woman* wife_;
};

class Woman : public Person {
public:
    Woman(const std::string& lastName, int age, double wealth)
        : Person(lastName, age, wealth), birthCooldown_(1), childrenCount_(0) {
        daughters.reserve(4);
    }

    void giveBirth(Man& husband, std::vector<Man>& sons, std::vector<Woman>& daughters);
    bool canGiveBirth() const { return birthCooldown_ == 0 && childrenCount_ < 3 && getAge() < 50; }
    void passOneYear() { if (birthCooldown_ > 0) --birthCooldown_; ageOneYear(); }
    void marry(Man& husband);
    std::vector<Marriage> marriages_;
    std::vector<Woman> daughters;

private:

    int birthCooldown_;  // 休产期
    int childrenCount_;  // 已生育孩子数量

protected:


};

class Man : public Person {
public:
    Man(const std::string& lastName, int age, double wealth) : Person(lastName, age, wealth)
    {
        sons.reserve(20);
    }
    bool marry(Woman& wife);
    bool isMarried(const Woman& wife) const;
    std::vector<Marriage> marriages_;
    std::vector<Man> sons;
private:

};

std::vector<Man> MenTotal;
std::vector<Woman> WomenTotal;
void InitializeGlobalVectors() {
    MenTotal.reserve(2000);
    WomenTotal.reserve(4000);
}



bool Man::marry(Woman& wife) {
    if (!isMarried(wife)) {
        Marriage marriage(this, &wife);
        marriages_.push_back(marriage);
        wife.marry(*this);
        return true;
    }
    return false;
}



bool Man::isMarried(const Woman& wife) const {
    for (const auto& marriage : marriages_) {
        if (marriage.getWife() == &wife) {
            return true;
        }
    }
    return false;
}


void Woman::marry(Man& husband) {
    Marriage marriage(&husband, this);
    marriages_.push_back(marriage);
}


void Woman::giveBirth(Man& husband, std::vector<Man>& sons, std::vector<Woman>& daughters) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> gender_dis(0, 1);
    std::uniform_real_distribution<> twin_dis(0, 1);

    if (birthCooldown_ == 0) {
        double gender = gender_dis(gen);
        bool isTwin = twin_dis(gen) <= 1.0 / 89;  // 1/89的概率生双胞胎

        for (int i = 0; i < (isTwin ? 2 : 1); ++i) {
            if (canGiveBirth()) {
                if (gender <= 0.33333) {
                    sons.emplace_back(husband.getLastName(), 0, 0.0);
                    MenTotal.emplace_back(husband.getLastName(), 0, 0.0);
                    sons.back().same = &MenTotal.back();
                    MenTotal.back().same = &sons.back();
                }
                else {
                    this->daughters.emplace_back(getLastName(), 0, 0.0);
                    WomenTotal.emplace_back(getLastName(), 0, 0.0);
                    daughters.back().same = &WomenTotal.back();
                    WomenTotal.back().same = &daughters.back();
                }

                this->childrenCount_ += isTwin ? 2 : 1; // 更新已生育孩子数量};
            }
        }
        this->birthCooldown_ = 3;
    }
}





int main() {

    InitializeGlobalVectors();
    //for (int j = 0; j < 100; j++) {
    MenTotal.emplace_back("Yang", 25, 700000.0);
    WomenTotal.emplace_back("Shen", 25, 700000.0);
    WomenTotal.emplace_back("Qin", 25, 700000.0);
    WomenTotal.emplace_back("Cai", 25, 700000.0);
    WomenTotal.emplace_back("Zhong", 27, 700000.0);

    MenTotal[0].marry(WomenTotal[0]);
    MenTotal[0].marry(WomenTotal[1]);
    MenTotal[0].marry(WomenTotal[2]);
    MenTotal[0].marry(WomenTotal[3]);


    int totalYears = 175; // 总共的年数
    for (int year = 1; year <= totalYears; ++year) {
        for (Woman& woman : WomenTotal) {
            // 检查女性是否结过婚
            if (woman.marriages_.size() == 1) {
                Man* husband = woman.marriages_[0].getHusband();
                woman.giveBirth(*husband, husband->sons, woman.daughters);
            }
        }


        // 在时间流逝时，增加 MenTotal 和 WomenTotal 中所有成员的年龄
        for (int i = 0; i < MenTotal.size(); ++i) {
            MenTotal[i].ageOneYear();
        }

        for (int j = 0; j < WomenTotal.size(); ++j) {
            WomenTotal[j].passOneYear();
        }

        
        // 遍历 MenTotal 中的男人
        for (Man& man : MenTotal) {
            if (man.getAge() >= 20 && man.marriages_.size() < 5) {
                // 遍历 WomenTotal 中的女人
                for (Woman& woman : WomenTotal) {
                    if (woman.getAge() >= 20 && woman.marriages_.size() < 1) {
                        // 让男人和女人结婚
                        man.marry(woman);
                    }
                }
            }
        }

        // 检查并处理男人的死亡和遗产分配
        auto menIter = MenTotal.begin();
        while (menIter != MenTotal.end()) {
            if (menIter->getAge() >= 85) {
                double inheritance = menIter->getWealth();
                int numSons = menIter->sons.size();
                if (numSons > 0) {
                    double eachShare = inheritance / numSons;
                    for (auto& son : menIter->sons) {
                        son.modifyWealth(eachShare);
                        if (son.same) {
                            son.same->modifyWealth(eachShare);  // 更新MenTotal中的数据
                        }
                    }
                }
                menIter = MenTotal.erase(menIter);
            }
            else {
                ++menIter;
            }
        }

        // 检查并处理女人的死亡和遗产分配
        auto womenIter = WomenTotal.begin();
        while (womenIter != WomenTotal.end()) {
            if (womenIter->getAge() >= 85) {
                double inheritance = womenIter->getWealth();
                int numDaughters = womenIter->daughters.size();
                if (numDaughters > 0) {
                    double eachShare = inheritance / numDaughters;
                    for (auto& daughter : womenIter->daughters) {
                        daughter.modifyWealth(eachShare);
                        if (daughter.same) {
                            daughter.same->modifyWealth(eachShare);  // 更新WomenTotal中的数据
                        }
                    }
                }
                womenIter = WomenTotal.erase(womenIter);
            }
            else {
                ++womenIter;
            }
        }


        
        // 定义年龄段的上限
        std::vector<int> ageUpperLimits = { 4, 9, 14, 19, 24, 29, 34, 39, 44, 49, 54, 59, 64, 69, 74, 79, 84 };
        int numGroups = ageUpperLimits.size(); // 年龄段的数量

        // 创建存储男人和女人数量的向量
        std::vector<int> menCounts(numGroups, 0);
        std::vector<int> womenCounts(numGroups, 0);

        // 遍历男人并统计每个年龄段的数量
        /*for (const Man& man : MenTotal) {
            int age = man.getAge();
            int groupIndex = age / 5;
            if (groupIndex < numGroups) {
                ++menCounts[groupIndex];
            }
        }*/

        // 遍历女人并统计每个年龄段的数量
        /*for (const Woman& woman : WomenTotal) {
            int age = woman.getAge();
            int groupIndex = age / 5;
            if (groupIndex < numGroups) {
                ++womenCounts[groupIndex];
            }
        }*/

        // 输出每个年龄段的男人和女人数量
        /*std::cout << "年份 " << year << "：" << std::endl;
        for (int group = 0; group < numGroups; ++group) {
            int lowerAge = group * 5;
            int upperAge = ageUpperLimits[group];
            std::cout << "年龄段 " << lowerAge << " - " << upperAge << " 岁的男人数量：" << menCounts[group]
                << ", 女人数量：" << womenCounts[group] << std::endl;
        }*/

        if (year == 175)
        {
            // 首先，按照年龄进行排序
            std::sort(MenTotal.begin(), MenTotal.end(), [](const Man& a, const Man& b) {
                return a.getAge() < b.getAge();
                });

            std::sort(WomenTotal.begin(), WomenTotal.end(), [](const Woman& a, const Woman& b) {
                return a.getAge() < b.getAge();
                });

            // 输出最后一年的男人财产
            std::cout << "男性财产:" << std::endl;
            for (const Man& man : MenTotal) {
                std::cout << "姓名: " << man.getLastName() << ", 年龄: " << man.getAge() << "岁, 财产: " << man.getWealth() << "元" << std::endl;
            }

            // 输出最后一年的女人财产
            std::cout << "女性财产:" << std::endl;
            for (const Woman& woman : WomenTotal) {
                std::cout << "姓名: " << woman.getLastName() << ", 年龄: " << woman.getAge() << "岁, 财产: " << woman.getWealth() << "元" << std::endl;
            }
        }
        menCounts.clear();
        womenCounts.clear();
    }

    return 0;

}
