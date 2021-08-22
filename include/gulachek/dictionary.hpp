#ifndef GULACHEK_DICTIONARY_HPP
#define GULACHEK_DICTIONARY_HPP

#include <gulachek/gtree.hpp>
#include <gulachek/gtree/encoding/map.hpp>
#include <gulachek/gtree/encoding/tree.hpp>
#include <gulachek/gtree/encoding/string.hpp>
#include <gulachek/gtree/encoding/class.hpp>

#include <map>
#include <string>

namespace gulachek
{
	template <typename Key>
	class basic_dictionary
	{
		public:
			using gtree_encoding = gtree::container_encoding;

			basic_dictionary() = default;

			template <typename MutableTree>
			gtree::error gtree_encode(MutableTree &tr) const
			{
				return gtree::encode(elems_, tr);
			}

			template <typename Tree>
			gtree::error gtree_decode(Tree &&tr)
			{
				return gtree::decode(std::forward<Tree>(tr), elems_);
			}

			template <typename T>
			gtree::error assign(const Key &key, T &&val)
			{
				return gtree::encode(std::forward<T>(val), elems_[key]);
			}

			template <typename T>
			gtree::error read(const Key &k, T &val) const
			{
				auto it = elems_.find(k);
				if (it == elems_.end())
					return {"Key not found"};

				return gtree::decode(it->second, val);
			}

		private:
			std::map<Key, gtree::mutable_tree> elems_;
	};

	using dictionary = basic_dictionary<std::string>;
}

#endif
