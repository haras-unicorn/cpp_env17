#ifndef ENV_BASE_DELETER_HPP
#define ENV_BASE_DELETER_HPP

tmp<name T = byte_t> typ(deleter_gt) = FWA_DELETER<T>;

typ(deleter_t) = deleter_gt<byte_t>;

#endif // ENV_BASE_DELETER_HPP
