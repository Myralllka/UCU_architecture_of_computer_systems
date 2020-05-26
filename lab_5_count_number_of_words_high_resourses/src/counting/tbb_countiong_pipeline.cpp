//
// Created by fenix on 5/22/20.
//
#include <tbb/pipeline.h>
#include <boost/locale.hpp>
#include <deque>

#include "../../includes/counting/tbb_countiong_pipeline.h"
#include "../../includes/files/file_packet.h"
#include "../../includes/files/file_interface.h"
#include "../../includes/archivation/archive_t.h"


namespace ba = boost::locale::boundary;

template<typename T>
class deallocator {
    const T *pointer;
public:
    explicit deallocator(T *pointer) : pointer(pointer) {}

    ~deallocator() { delete pointer; }
};

std::map<std::string, size_t> tbb_count_words(const std::vector<std::string> &first) {
    const size_t ntoken = 30;
    auto next_file = first.begin() - 1;
    std::map<std::string, size_t> res_map{};

    /////////////////////////////////// Load ///////////////////////////////////////
    tbb::filter_t<void, file_packet *>
            load(tbb::filter::serial_out_of_order,

                 [&](tbb::flow_control &fc) -> file_packet * {
                     auto *packet_p = new file_packet{};
                     if (++next_file < first.end())
                         *packet_p = read_input_file_gen(*next_file);
                     else
                         fc.stop();
                     return packet_p;
                 }
    );
    /////////////////////////////////// Count ///////////////////////////////////////
    tbb::filter_t<file_packet *, std::map<std::string, size_t> *>
            count(tbb::filter::parallel,

                  [](file_packet *packet) -> std::map<std::string, size_t> * {
                      deallocator packet_free{packet};
                      auto *local_map = new std::map<std::string, size_t>{};
                      if (packet->empty()) {
                          return local_map;
                      }

                      std::string tmp_content;
                      std::deque<std::string> data_q;
                      if (packet->archived) {
                          archive_t tmp_archive{std::move(packet->content)};
                          tmp_archive.extract_all(data_q);
                      } else {
                          data_q.emplace_back(std::move(packet->content));
                      }
                      while (!data_q.empty()) {
                          tmp_content = std::move(data_q.front());
                          data_q.pop_front();
                          tmp_content = boost::locale::to_lower(
                                  boost::locale::fold_case(boost::locale::normalize(tmp_content)));
                          ba::ssegment_index map(ba::word, tmp_content.begin(), tmp_content.end());
                          map.rule(ba::word_letters);
                          for (auto it = map.begin(), e = map.end(); it != e; ++it)
                              ++(*local_map)[*it];
                          tmp_content.clear();
                      }
                      return local_map;
                  }
    );
    /////////////////////////////////// Merge ///////////////////////////////////////
    tbb::filter_t<std::map<std::string, size_t> *, void>
            merge(tbb::filter::serial_out_of_order,

                  [&](std::map<std::string, size_t> *map) {
                      deallocator map_free{map};
                      for (const auto &element : *map)
                          res_map[element.first] += (*map)[element.first];
                  });

    //////////////////////////////////// Run ////////////////////////////////////////
    tbb::filter_t<void, void> pipeline = load & count & merge;
    tbb::parallel_pipeline(ntoken, pipeline);
    return std::move(res_map);
}