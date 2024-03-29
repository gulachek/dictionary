#ifndef GULACHEK_DICTIONARY_HPP
#define GULACHEK_DICTIONARY_HPP

#include <gulachek/gtree.hpp>
#include <gulachek/gtree/encoding/map.hpp>
#include <gulachek/gtree/encoding/string.hpp>

#include <map>
#include <string>
#include <sstream>
#include <strstream>

namespace gulachek
{
	template <typename Key>
	class basic_dictionary
	{
		using map_type = std::map<Key, gtree::tree>;

		public:
			basic_dictionary() = default;

			error gtree_encode(gtree::tree_writer &w) const
			{
				return w.write(elems_);
			}

			error gtree_decode(gtree::treeder &r)
			{
				gtree::decoding<map_type> dec{&elems_};
				return dec.decode(r);
			}

			template <gtree::encodable T>
			error assign(const Key &key, const T &val)
			{
				auto &tr = elems_[key];

				if (auto err = tr.write(val))
				{
					error wrap{"error writing value"};
					if constexpr (error_writable<Key>)
					{
						wrap << " for key " << key;
					}
					wrap.add_cause(err);
					return wrap;
				}

				return {};
			}

			template <gtree::encodable T>
			error read(const Key &k, T *val) const
			{
				auto it = elems_.find(k);
				if (it == elems_.end())
					return {"Key not found"};

				const auto &tr = it->second;

				if (auto err = tr.read(val))
				{
					error wrap{"error reading value"};
					if constexpr (error_writable<Key>)
					{
						wrap << " for key " << k;
					}
					wrap.add_cause(err);
					return wrap;
				}

				return {};
			}

			std::size_t size() const
			{ return elems_.size(); }

		private:
			map_type elems_;
	};

	using dictionary = basic_dictionary<std::string>;
}

#endif
