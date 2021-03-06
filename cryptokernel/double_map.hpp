// Author: Dmitry Kukovinets (d1021976@gmail.com)

// Hashers
template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
inline
size_t
double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::hash1::operator()(const Key1 *k) const
{ return this->Hash1::operator()(*k); }

template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
inline
size_t
double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::hash2::operator()(const Key2 *k) const
{ return this->Hash2::operator()(*k); }


// Predictors
template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
inline
bool
double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::pred1::operator()(const Key1 *k1, const Key1 *k2) const
{ return this->Pred1::operator()(*k1, *k2); }

template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
inline
bool
double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::pred2::operator()(const Key2 *k1, const Key2 *k2) const
{ return this->Pred2::operator()(*k1, *k2); }


// Iterators
template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
inline
typename double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::const_iterator
double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::begin() const
{
	return this->list_.begin();
}

template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
inline
typename double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::const_iterator
double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::end() const
{
	return this->list_.end();
}


// Constructors and operator=()
template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
inline
double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::double_map()											// Default
{}

template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
inline
double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::double_map(const double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2> &other):	// Copy
	list_(other.list_),
	map1_(other.map1_.bucket_count()),
	map2_(other.map2_.bucket_count())
{
	for (auto it = this->list_.begin(), end = this->list_.end(); it != end; ++it) {
		this->map1_.insert(std::move(std::make_pair(&it->first, it)));
		this->map2_.insert(std::move(std::make_pair(&it->second, it)));
	}
}

template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
inline
double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::double_map(double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2> &&other):		// Move
	list_(std::move(other.list_)),
	map1_(std::move(other.map1_)),
	map2_(std::move(other.map2_))
{}


template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
inline
double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2> &
double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::operator=(const double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2> &other)	// Copy
{
	if (this == &other) return *this;
	this->map1_.clear();
	this->map2_.clear();
	this->list_ = other.list_;
	for (auto it = this->list_.begin(), end = this->list_.end(); it != end; ++it) {
		this->map1_.insert(std::move(std::make_pair(&it->first, it)));
		this->map2_.insert(std::move(std::make_pair(&it->second, it)));
	}
	return *this;
}

template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
inline
double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2> &
double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::operator=(double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2> &&other)			// Move
{
	this->list_ = std::move(other.list_);
	this->map1_ = std::move(other.map1_);
	this->map2_ = std::move(other.map2_);
	return *this;
}


template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
std::pair<typename double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::const_iterator, bool>
double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::insert(const triple<Key1, Key2, Value> &t)
{
	if (this->map1_.find(&t.first) != this->map1_.end()
		|| this->map2_.find(&t.second) != this->map2_.end())
		return std::move(std::make_pair(std::move(this->end()), false));
	
	this->list_.push_back(t);
	auto it = --(this->list_.end());
	
	this->map1_[&it->first] = it;
	this->map2_[&it->second] = it;
	return std::make_pair(std::move(it), true);
}

template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
std::pair<typename double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::const_iterator, bool>
double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::insert(triple<Key1, Key2, Value> &&t)
{
	if (this->map1_.find(&t.first) != this->map1_.end()
		|| this->map2_.find(&t.second) != this->map2_.end())
		return std::move(std::make_pair(std::move(this->end()), false));
	
	this->list_.push_back(std::move(t));
	auto it = --(this->list_.end());
	
	this->map1_[&it->first] = it;
	this->map2_[&it->second] = it;
	return std::make_pair(std::move(it), true);
}


// Like random_map::insert_random()
// Generates Key1 automatically.
template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
template<class Generator>
std::pair<typename double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::const_iterator, bool>
double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::insert_random1(const Generator &g, const Key2 &k2, const Value &val)
{
	auto end = this->end();
	auto it2 = this->map2_.find(&k2);
	if (it2 != this->map2_.end()) return std::make_pair(std::move(end), false);
	
	auto end1 = this->map1_.end();
	while (true) {
		Key1 k1 = g();
		if (g.bad())
			return std::make_pair(std::move(end), false);
		auto it1 = this->map1_.find(&k1);
		if (it1 == end1) {
			auto it = this->list_.insert(end, std::move(make_triple(k1, k2, val)));
			this->map1_[&it->first] = it;
			this->map2_[&it->second] = it;
			return std::make_pair(std::move(it), true);
		}
	}
}

// Generates Key2 automatically.
template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
template<class Generator>
std::pair<typename double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::const_iterator, bool>
double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::insert_random2(const Generator &g, const Key1 &k1, const Value &val)
{
	auto end = this->end();
	auto it1 = this->map1_.find(&k1);
	if (it1 != this->map1_.end()) return std::make_pair(std::move(end), false);
	
	auto end2 = this->map2_.end();
	while (true) {
		Key2 k2 = g();
		if (g.bad())
			return std::make_pair(std::move(end), false);
		auto it2 = this->map2_.find(&k2);
		if (it2 == end2) {
			auto it = this->list_.insert(end, std::move(make_triple(k1, k2, val)));
			this->map1_[&it->first] = it;
			this->map2_[&it->second] = it;
			return std::make_pair(std::move(it), true);
		}
	}
}

// Generates both keys automatically.
template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
template<class Generator>
std::pair<typename double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::const_iterator, bool>
double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::insert_random(const Generator &g, const Value &val)
{
	// Generating k1
	auto end1 = this->map1_.end(), it1 = end1;
	Key1 k1;
	do {
		k1 = g();
		it1 = this->map1_.find(&k1);
	} while (it1 != end1);
	
	// Generating k2
	auto end2 = this->map2_.end(), it2 = end2;
	Key2 k2;
	do {
		k2 = g();
		it2 = this->map2_.find(&k2);
	} while (it2 != end2);
	
	auto it = this->list_.insert(this->list_.end(), std::move(make_triple(k1, k2, val)));
	this->map1_[&it->first] = it;
	this->map2_[&it->second] = it;
	return std::make_pair(std::move(it), true);
}


template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
void
double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::erase(double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::const_iterator it)
{
	if (it == this->end()) return;
	
	this->map1_.erase(&it->first);
	this->map2_.erase(&it->second);
	this->list_.erase(it);
}

template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
inline
void
double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::erase1(const Key1 &k1)
{
	this->erase(this->find1(k1));
}

template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
inline
void
double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::erase2(const Key2 &k2)
{
	this->erase(this->find2(k2));
}


// Updates 1st or 2nd key and returns true. If it's impossible (new_k already
// exists or k1 does not exists), returns false and does NOT change any element.
template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
bool
double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::update1(const Key1 &k1, const Key1 &new_k1)
{
	auto it1 = this->map1_.find(&k1);
	if (it1 == this->map1_.end()							// Old element not found
		|| this->map1_.find(&new_k1) != this->map1_.end())	// Or the new one already exists
		return false;
	
	auto it = it1->second;
	this->map1_.erase(it1);	// Cleaning old data from map1_
	it->first = new_k1;
	this->map1_.emplace(&(it->first), it);	// Inserting new data into map1_
	return true;
}

template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
bool
double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::update2(const Key2 &k2, const Key2 &new_k2)
{
	auto it2 = this->map2_.find(&k2);
	if (it2 == this->map2_.end()							// Old element not found
		|| this->map2_.find(&new_k2) != this->map2_.end())	// Or the new one already exists
		return false;
	
	auto it = it2->second;
	this->map2_.erase(it2);	// Cleaning old data from map2_
	it->second = new_k2;
	this->map2_.emplace(&(it->second), it);	// Inserting new data into map2_
	return true;
}


// As update1/update2, but use one key to update another.
template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
bool
double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::update12(const Key1 &k1, const Key2 &new_k2)
{
	auto it = this->find1(k1);
	if (it == this->end()) return false;
	return this->update2(it->second, new_k2);
}

template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
bool
double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::update21(const Key2 &k2, const Key1 &new_k1)
{
	auto it = this->find2(k2);
	if (it == this->end()) return false;
	return this->update1(it->first, new_k1);
}


template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
inline
size_t
double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::size() const
{
	// Not this->list_.size(), because it can be O(n).
	return this->map1_.size();
}

template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
inline
void
double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::clear()
{
	this->map1_.clear();
	this->map2_.clear();
	this->list_.clear();
}


template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
typename double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::const_iterator
double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::find1(const Key1 &k1) const
{
	auto it = this->map1_.find(&k1);
	if (it == this->map1_.end()) return this->end();
	return it->second;
}

template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
typename double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::const_iterator
double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::find2(const Key2 &k2) const
{
	auto it = this->map2_.find(&k2);
	if (it == this->map2_.end()) return this->end();
	return it->second;
}


template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
inline
Value &
double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::at1(const Key1 &k1)
{ return this->map1_.at(&k1)->third; }

template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
inline
const Value &
double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::at1(const Key1 &k1) const
{ return this->map1_.at(&k1)->third; }


template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
Value &
double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::at2(const Key2 &k2)
{ return this->map2_.at(&k2)->third; }

template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
const Value &
double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::at2(const Key2 &k2) const
{ return this->map2_.at(&k2)->third; }


// Returns true, if key exists, or false otherwise
template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
bool
double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::test1(const Key1 &k1) const
{
	auto it = this->find1(k1);
	if (it == this->end()) return false;
	return true;
}

template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
bool
double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::test2(const Key2 &k2) const
{
	auto it = this->find2(k2);
	if (it == this->end()) return false;
	return true;
}


template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
std::vector<Key1>
double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::all_keys1() const
{
	std::vector<Key1> res;
	res.reserve(this->size());
	for (auto &x: *this)
		res.push_back(x.first);
	return res;
}

template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
std::vector<Key2>
double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::all_keys2() const
{
	std::vector<Key2> res;
	res.reserve(this->size());
	for (auto &x: *this)
		res.push_back(x.second);
	return res;
}
