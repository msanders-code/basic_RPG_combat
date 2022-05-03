#include <iostream>
#include <cmath>
#include <vector>
#include <queue>
#include <string>


class Attack {
	std::string m_name;
	double m_attackStat;

public:
	// Initializer
	Attack(std::string name, double attackStat) {
		m_name = name;
		m_attackStat = attackStat;
	}

	std::string getName() {
		return m_name;
	}

	double getAttackStat() {
		return m_attackStat;
	}

	void setAttackStat(double attackStrength) {
		m_attackStat = attackStrength;
	}
};


class Item {
	std::string m_name;
	double m_healStat;

public:
	// Initializer
	Item(std::string name, double healStat) {
		m_name = name;
		m_healStat = healStat;
	}

	std::string getName() {
		return m_name;
	}

	double getHealStat() {
		return m_healStat;
	}

	void setHealStat(double healPoints) {
		m_healStat = healPoints;
	}
};


class Character {

	std::string m_name;
	double health = 100.0;
	int strengthMultiplier;
	int defenseMultiplier;

	// Searches item array for specified item
	double findItem(std::string itemName) {

		for (auto item : itemSet) {

			if (item.getName() == itemName) {
				return item.getHealStat();
			}
		}

		return 0.0;
	}
	

public:
	// Initializer
	Character(std::string name, int strength, int defense, std::vector<Attack>& attacks, std::vector<Item>& items) {
		m_name = name;
		strengthMultiplier = strength;
		defenseMultiplier = defense;
		attackSet = attacks;
		itemSet = items;
	}

	std::string getName() {
		return m_name;
	}

	double getHealth(){
		return health;
	}

	int getStrengthMultiplier() {
		return strengthMultiplier;
	}

	int getDefenseMultiplier() {
		return defenseMultiplier;
	}

	std::vector<Attack>& getAttackList() {
		return attackSet;
	}

	virtual std::vector<Item>& getItemList() {
		return itemSet;
	}

	void setStrengthMultiplier(int newMultiplier) {
		strengthMultiplier = newMultiplier;
	}

	void setDefenseMultiplier(int newMultiplier) {
		defenseMultiplier = newMultiplier;
	}

	// Method to attack another Character
	void doAttack(std::string attackName, Character& other) {

		std::cout << m_name << " attacks " << other.getName();
		std::cout << " using " << attackName << std::endl;
		
		if (strengthMultiplier > 0) {
			other.reactToAttack(findAttack(attackName) * strengthMultiplier);
		}
		else if (strengthMultiplier < 0){
			other.reactToAttack(findAttack(attackName) / std::abs(strengthMultiplier));
		}
		else {
			other.reactToAttack(findAttack(attackName));
		}

		if (other.getHealth() <= 0) {
			std::cout << std::endl << m_name << " has killed " << other.getName() << "!\n\n";
		}

	}

	virtual void useItem(std::string itemName) {

		std::cout << m_name << " uses " << itemName << std::endl;

		health += findItem(itemName);
	}

	// Prints player stats to the console
	virtual void printStats() {
		std::cout << m_name << std::endl;
		std::cout << "\tName: " << m_name << std::endl;
		std::cout << "\tCurrent Health: " << health << std::endl;
		std::cout << "\tAttacks: ";

		for (auto attack : attackSet) {
			std::cout << attack.getName() << ", ";
		}
		std::cout << std::endl;

		std::cout << "\tItems: ";

		for (auto item : itemSet) {
			std::cout << item.getName() << ", ";
		}
		std::cout << std::endl;

		std::cout << "\tStrength Multiplier: " << strengthMultiplier << std::endl;
		std::cout << "\tDefense Multiplier: " << defenseMultiplier << std::endl;
		std::cout << std::endl;
	}

protected:

	std::vector<Attack> attackSet;  // Set of attacks for the character
	std::vector<Item> itemSet;  // Set of items for the character

	// Updates a Characters health based on attack
	void reactToAttack(double attackStrength) {
		
		if (defenseMultiplier > 0) {
			health -= (attackStrength / defenseMultiplier);
		}
		else if (defenseMultiplier < 0) {
			health -= (attackStrength * std::abs(defenseMultiplier));
		}
		else {
			health -= attackStrength;
		}
	}

	// Searches attack array for specified attack
	double findAttack(std::string attackName) {

		for (auto attack : attackSet) {

			if (attack.getName() == attackName) {
				return attack.getAttackStat();
			}
		}

		return 0.0;
	}
};

// Enemy class that inherits from the character class
class Enemy : public Character {

public:
	// Initializer inherited from Character class
	using Character::Character;

	// Overridden display function for enemy characters
	void printStats() override {
		std::cout << this->getName();
		std::cout << " - Current Health: " << this->getHealth() << std::endl;
	}

private:
	// Returns no items, because enemies don't have items
	std::vector<Item>& getItemList() override { 
		return this->itemSet; 
	}

	// Enemies don't have any items to use
	void useItem(std::string itemName) override {}

};


void playGame(Character& player, std::queue<Enemy>& enemies) {

	std::string userChoice;
	
	// Prints starting player information
	std::cout << "Player:\n\n";
	player.printStats();
	std::cout << std::endl;
	
	std::cout << player.getName() << " Finds himself trapped in a dungeon!\n";
	std::cout << "There's a series of rooms in front of him, he enters a room...\n\n";
	
	// Start game loop
	while (!enemies.empty() && player.getHealth() > 0.0) {
		std::cout << "A monster appears! It looks like a " << enemies.front().getName() << std::endl;

		// Loop while the current enemy still has health
		while (enemies.front().getHealth() > 0.0 && player.getHealth() > 0.0) {

			// Print the enemies stats and the players current health
			enemies.front().printStats();
			std::cout << "\nyour current health: " << player.getHealth() << std::endl;

			std::cout << "\nEnter 1 to attack or 2 to use an item and prepare for attack:\n";
			std::getline(std::cin, userChoice);


			// Options for player choosing to attack
			if (userChoice.compare("1") == 0) {

				std::cout << "You've chosen to attack!\n\nAttacks:\n\n";
				int i = 1;

				// Display attack options
				for (auto attack : player.getAttackList()) {
					std::cout << i << ". " << attack.getName() << std::endl;
					i++;
				}
				std::cout << std::endl;

				std::cout << "Choose which attack you'd like to use (1 or 2): ";
				std::getline(std::cin, userChoice);

				// Attack the monster with the player's chosen attack
				if (userChoice.compare("1") == 0) {
					player.doAttack("Sword To The Face", enemies.front());
				}
				else if (userChoice.compare("2") == 0) {
					player.doAttack("Stab To The Heart", enemies.front());
				}
				else {
					std::cout << "Not a valid choice, try again\n\n";
					continue;
				}
			}
			// Options for player choosing to use an item
			else if (userChoice.compare("2") == 0) {
				
				std::cout << "You've chosen to use an Item!\n\nItems:\n\n";
				int i = 1;

				// Display item options
				for (auto item : player.getItemList()) {
					std::cout << i << ". " << item.getName() << std::endl;
					i++;
				}
				std::cout << std::endl;

				std::cout << "Choose which item you'd like to use (1 or 2): ";
				std::getline(std::cin, userChoice);

				// Use the player's chosen item to increase health
				if (userChoice.compare("1") == 0) {
					player.useItem("Grog");
				}
				else if (userChoice.compare("2") == 0) {
					player.useItem("Roasted Meat");
				}
				else {
					std::cout << "Not a valid choice, try again\n\n";
					continue;
				}

			}
			else {
				std::cout << "Not a valid choice, try again\n\n";
				continue;
			}

			// Enemy retaliates with an attack if it's not been killed 
			if (enemies.front().getHealth() > 0) {
				
				std::cout << enemies.front().getName() << " retaliates with an attack!\n";
				int randAttack = rand() % 2;  // Generate a 0 or 1 to pick an attack at random

				// Enemy attacks player
				std::cout << std::endl;
				enemies.front().doAttack(enemies.front().getAttackList()[randAttack].getName(), player);
			}
		}

		if (player.getHealth() > 0.0) {
			// Let the player enter the next room after defeating the current monster
			std::cout << "\nHe enters the next room...";
			enemies.pop();
		}
	}
	
	// End of game results
	if (player.getHealth() <= 0.0) {
		std::cout << "\nYou have lost..\n";
	}
	else {
		std::cout << "\nYou have won!\n";
	}
}

int main() {

	// Sets of character and enemy attacks
	std::vector<Attack> heroAttacks = { {"Sword To The Face", 25.4}, {"Stab To The Heart", 30.0} };
	std::vector<Attack> dragonAttacks = { {"Flame Breath", 30.6}, {"Tail Whip", 28.0} };
	std::vector<Attack> hydraAttacks = { {"Fireball", 30.0}, {"Bite", 23.0} };
	std::vector<Attack> slimeAttacks = { {"Ectoplasm", 25.4}, {"Slime Ball", 14.9} };

	// Sets of hero and enemy items
	std::vector<Item> heroItems = { {"Grog", 15.9}, {"Roasted Meat", 20.0} };
	std::vector<Item> enemyItems;

	// Hero character
	Character hero("Sir Rodrick", 20, 10, heroAttacks, heroItems);
	
	// Enemies
	Enemy dragon("Dragon", 5, 12, dragonAttacks, enemyItems);
	Enemy hydra("Hydra", 5, 10, hydraAttacks, enemyItems);
	Enemy slime("Slime", 0, 5, slimeAttacks, enemyItems);

	// Queue of enemies
	std::queue<Enemy> enemyList;
	enemyList.push(dragon);
	enemyList.push(hydra);
	enemyList.push(slime);

	// Start game
	playGame(hero, enemyList);

	return 0;
}