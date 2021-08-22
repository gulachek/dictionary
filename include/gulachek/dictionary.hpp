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
				tr.child_count(elems_.size());

				std::size_t i = 0;
				for (const auto &key_elem : elems_)
				{
					const auto &elem = key_elem.second;
					if (auto err = gtree::encode(elem, tr.child(i)))
						return err;
					++i;
				}

				return {};
			}

			template <typename Tree>
			gtree::error gtree_decode(Tree &&tr)
			{
				for (std::size_t i = 0; i < tr.child_count(); ++i)
				{
					Key k;
					if (auto err = gtree::decode(tr.child(i), k))
						return err;

					if (auto err = gtree::decode(tr.child(i), elems_[k]))
						return err;
				}

				return {};
			}

			template <typename T>
			gtree::error assign(const Key &key, T &&val)
			{
				auto &elem = elems_[key];
				if (auto err = gtree::encode(key, elem))
					return err;

				if (gtree::uses_value<T>::value)
				{
					elem.child_count(1);
					return gtree::encode(std::forward<T>(val), elem.child(0));
				}
				else
				{
					return gtree::encode(std::forward<T>(val), elem);
				}
			}

			template <typename T>
			gtree::error read(const Key &k, T &val) const
			{
				auto it = elems_.find(k);
				if (it == elems_.end())
					return {"Key not found"};

				const auto &elem = it->second;

				if (gtree::uses_value<T>::value)
				{
					if (elem.child_count() < 1)
						return {"Expected child of element which uses_value"};

					return gtree::decode(elem.child(0), val);
				}
				else
				{
					return gtree::decode(elem, val);
				}
			}

			std::size_t size() const
			{ return elems_.size(); }

		private:
			std::map<Key, gtree::mutable_tree> elems_;
	};

	using dictionary = basic_dictionary<std::string>;
}

#endif
