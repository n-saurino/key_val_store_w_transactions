#pragma once
#include <string>
#include <unordered_map>
#include <vector>

namespace Constants{
	constexpr const char* DELETED = "__DELETED__";
}

class KeyValueStore{
public:
	KeyValueStore(){

	}

	~KeyValueStore(){

	}
	
	void Set(const std::string& key, const std::string& value){
		if(!transactions_.empty()){
			transactions_.back()[key] = value;
		}else{
			store_[key] = value;
		}	
	}

	std::string Get(const std::string& key){
		for(auto it = transactions_.rbegin(); it != transactions_.rend(); ++it){
			if(it->find(key) != it->end()){
				return it->at(key) == Constants::DELETED ? "NULL" : it->at(key);
			}
		}	
		return store_.find(key) == store_.end() ? "NULL" : store_[key];
	}
	
	void Delete(const std::string& key){
		if(!transactions_.empty()){
			transactions_.back()[key] = Constants::DELETED; 
		}else{
			store_.erase(key);
		}	
	}
	
	void Begin(){
		transactions_.emplace_back();	
	}
	
	bool Commit(){
		if(transactions_.empty()){
			return false;
		}	
		auto transaction_set{std::move(transactions_.back())};
		transactions_.pop_back();
		for(const auto& [key, value] : transaction_set){
			if(!transactions_.empty()){
				transactions_.back()[key] = value;
			}else{
				if(value == Constants::DELETED){
					store_.erase(key);
				}else{
					store_[key] = value;
				}
			}
		}

		return true;
	}
	
	bool Rollback(){
		if(transactions_.empty()){
			return false;	
		}	
		
		transactions_.pop_back();
		return true;
	}

private:
	std::unordered_map<std::string, std::string> store_{};
	std::vector<std::unordered_map<std::string, std::string>> transactions_{};
};
