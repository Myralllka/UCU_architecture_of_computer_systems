//
// Created by fenix on 5/22/20.
//


#include <iostream>
#include <chrono>
#include <boost/locale.hpp>
#include <filesystem>
#include <thread>

#include "includes/files/file_interface.h"
#include "includes/files/config_file.h"
#include "includes/speed_tester.h"
#include "includes/counting/parallel_program.h"
#include "tbb/concurrent_queue.h"
#include "tbb/pipeline.h"
#include "includes/counting/linear_program.h"
#include <cinttypes>
#include <cassert>

#define MAX_CHAR_PER_INPUT_SLICE 4000

// Holds a slice of text.
/** Instances *must* be allocated/freed using methods herein, because the C++ declaration
   represents only the header of a much larger object in memory. */
class TextSlice {
    // Pointer to one past last character in sequence
    char *logical_end;
    // Pointer to one past last available byte in sequence.
    char *physical_end;
public:
    // Allocate a TextSlice object that can hold up to max_size characters.
    static TextSlice *allocate(size_t max_size) {
        // +1 leaves room for a terminating null character.
        TextSlice *t = (TextSlice *) tbb::tbb_allocator<char>().allocate(sizeof(TextSlice) + max_size + 1);
        t->logical_end = t->begin();
        t->physical_end = t->begin() + max_size;
        return t;
    }

    // Free this TextSlice object
    void free() {
        tbb::tbb_allocator<char>().deallocate((char *) this, sizeof(TextSlice) + (physical_end - begin()) + 1);
    }

    // Pointer to beginning of sequence
    char *begin() { return (char *) (this + 1); }

    // Pointer to one past last character in sequence
    char *end() { return logical_end; }

    // Length of sequence
    [[nodiscard]] size_t size() const { return logical_end - (char *) (this + 1); }

    // Maximum number of characters that can be appended to sequence
    [[nodiscard]] size_t avail() const { return physical_end - logical_end; }

    // Append sequence [first,last) to this sequence.
    void append(char *first, char *last) {
        memcpy(logical_end, first, last - first);
        logical_end += last - first;
    }

    // Set end() to given value.
    void set_end(char *p) { logical_end = p; }
};

// Functor that writes a TextSlice to a file.
class MyOutputFunc {
    FILE *my_output_file;
public:
    explicit MyOutputFunc(FILE *output_file) : my_output_file(output_file) {}

    void operator()(TextSlice *out) const {
        size_t n = fwrite(out->begin(), 1, out->size(), my_output_file);
        if (n != out->size()) {
            fprintf(stderr, "Can't write into file\n");
            exit(1);
        }
        out->free();
    }
};

// Functor that changes each decimal number to its square.
class MyTransformFunc {
public:
    TextSlice *operator()(TextSlice *input) const {
        // Add terminating null so that strtol works right even if number is at end of the input.
        *input->end() = '\0';
        char *p = input->begin();
        TextSlice *out = TextSlice::allocate(2 * MAX_CHAR_PER_INPUT_SLICE);
        char *q = out->begin();
        for (;;) {
            while (p < input->end() && !isdigit(*p))
                *q++ = *p++;
            if (p == input->end())
                break;
            long x = strtol(p, &p, 10);
            // Note: no overflow checking is needed here, as we have twice the
            // input string length, but the square of a non-negative integer n
            // cannot have more than twice as many digits as n.
            long y = x * x;
            sprintf(q, "%ld", y);
            q = strchr(q, 0);
        }
        out->set_end(q);
        input->free();
        return out;
    }
};


class MyInputFunc {
    static TextSlice *next_slice;
public:
    explicit MyInputFunc(FILE *input_file_) : input_file(input_file_) {}

    MyInputFunc(const MyInputFunc &f) = default;

    ~MyInputFunc() = default;

    TextSlice *operator()(tbb::flow_control &fc) const {
        // Read characters into space that is available in the next slice.
        if (!next_slice)
            next_slice = TextSlice::allocate(MAX_CHAR_PER_INPUT_SLICE);
        size_t m = next_slice->avail();
        size_t n = fread(next_slice->end(), 1, m, input_file);
        if (!n && next_slice->size() == 0) {
            // No more characters to process
            fc.stop();
            return nullptr;
        } else {
            // Have more characters to process.
            TextSlice *t = next_slice;
            next_slice = TextSlice::allocate(MAX_CHAR_PER_INPUT_SLICE);
            char *p = t->end() + n;
            if (n == m) {
                // Might have read partial number.
                // If so, transfer characters of partial number to next slice.
                while (p > t->begin() && isdigit(p[-1]))
                    --p;
                assert(p > t->begin() && "Number too large to fit in buffer.\n");
                next_slice->append(p, t->end() + n);
            }
            t->set_end(p);
            return t;
        }
    }

private:
    FILE *input_file;
};

TextSlice *MyInputFunc::next_slice = nullptr;


void RunPipeline(int ntoken, FILE *input_file, FILE *output_file) {
    tbb::parallel_pipeline(
            ntoken,
            tbb::make_filter<void, TextSlice *>(tbb::filter::serial_in_order, MyInputFunc(input_file))
            &
            tbb::make_filter<TextSlice *, TextSlice *>(tbb::filter::parallel, MyTransformFunc())
            &
            tbb::make_filter<TextSlice *, void>(tbb::filter::serial_in_order, MyOutputFunc(output_file)));
}

float RootMeanSquare(float *first, float *last) {
    using namespace tbb;
    float sum = 0;
    parallel_pipeline(16,
                      make_filter<void, float *>(filter::serial,
                                                 [&](flow_control &fc) -> float * {
                                                     if (first < last) {
                                                         return first++;
                                                     } else {
                                                         fc.stop();
                                                         return nullptr;
                                                     }
                                                 }
                      ) &
                      make_filter<float *, float>(filter::parallel, [](float *p) { return (*p) * (*p); }
                      ) &
                      make_filter<float, void>(filter::serial, [&](float x) { sum += x; }
                      )
    );
    return sqrt(sum);
}

int main(int argc, char *argv[]) {

    return 0;
}
