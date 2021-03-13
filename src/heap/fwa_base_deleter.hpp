#ifndef FWA_CORE_BASE_DELETER_HPP
#define FWA_CORE_BASE_DELETER_HPP

tmp<name T = byte_t> typ(deleter_gt) = FWA_DELETER<T>;

typ(deleter_t) = deleter_gt<byte_t>;

#endif // FWA_CORE_BASE_DELETER_HPP
