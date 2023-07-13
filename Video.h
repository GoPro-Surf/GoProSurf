#ifndef GOPROSURFSPLITTER_VIDEO_H
#define GOPROSURFSPLITTER_VIDEO_H

#include <string>
#include <stdexcept>
#include <array>
#include <vector>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}

class AvException : public std::logic_error {
public:
    explicit AvException(const std::string &message, int avError) : logic_error(format(message, avError)) {}

private:
    static std::string format(const std::string &message, int avError) {
        char buf[AV_ERROR_MAX_STRING_SIZE];
        return message + ": " + av_make_error_string(buf, AV_ERROR_MAX_STRING_SIZE, avError);
    }
};

class Video {
public:
    static void CopyFromTo(std::string &fromFile, std::string &toFile, size_t from, size_t to) {
        AVFormatContext *inputFormatContext = nullptr;

        auto ret = avformat_open_input(&inputFormatContext, fromFile.c_str(), nullptr, nullptr);
        if (ret != 0)
            throw AvException("Error opening the file " + fromFile, ret);

        ret = avformat_find_stream_info(inputFormatContext, nullptr);
        if (ret < 0)
            throw AvException("Could not find stream information in file " + fromFile, ret);

        AVFormatContext *outputFormatContext = nullptr;
        ret = avformat_alloc_output_context2(&outputFormatContext, nullptr, nullptr, toFile.c_str());
        if (ret < 0)
            throw AvException("Error creating output format context to file " + toFile, ret);

        for (auto i = 0; i < inputFormatContext->nb_streams; ++i) {
            AVStream *inStream = inputFormatContext->streams[i];
            if (inStream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO ||
                inStream->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
                AVStream *outStream = avformat_new_stream(outputFormatContext, nullptr);
                if (!outStream)
                    throw std::invalid_argument("Failed to allocate output stream");

                ret = avcodec_parameters_copy(outStream->codecpar, inStream->codecpar);
                if (ret < 0)
                    throw AvException("Failed to copy codec parameters", ret);
            }
        }

        av_dump_format(outputFormatContext, 0, toFile.c_str(), 1);

        int64_t startTime = int64_t(from) * AV_TIME_BASE;
        int64_t duration = int64_t(to - from) * AV_TIME_BASE;

        int64_t seekTarget = startTime;
        ret = avformat_seek_file(inputFormatContext, -1, INT64_MIN, seekTarget, INT64_MAX, 0);
        if (ret < 0)
            throw AvException("Failed to seek to the start time", ret);

        outputFormatContext->duration = duration;

        ret = avio_open(&outputFormatContext->pb, toFile.c_str(), AVIO_FLAG_WRITE);
        if (ret < 0)
            throw AvException("Could not open output file " + toFile, ret);

        ret = avformat_write_header(outputFormatContext, nullptr);
        if (ret < 0)
            throw AvException("Failed to write output file header to file " + toFile, ret);

        AVPacket packet;
        std::vector<int64_t> dtsStartFrom(inputFormatContext->nb_streams);
        std::vector<int64_t> ptsStartFrom(inputFormatContext->nb_streams);

        while (true) {
            ret = av_read_frame(inputFormatContext, &packet);
            if (ret == AVERROR_EOF)
                break;
            if (ret < 0)
                throw AvException("Error reading frame", ret);

            if (av_q2d(inputFormatContext->streams[packet.stream_index]->time_base) * double(packet.pts) > double(to)) {
                av_packet_unref(&packet);
                break;
            }

            if (packet.stream_index > 1) {
                av_packet_unref(&packet);
                continue;
            }

            if (dtsStartFrom[packet.stream_index] == 0)
                dtsStartFrom[packet.stream_index] = packet.dts;
            if (ptsStartFrom[packet.stream_index] == 0)
                ptsStartFrom[packet.stream_index] = packet.pts;

            packet.dts -= dtsStartFrom[packet.stream_index];
            packet.pts -= ptsStartFrom[packet.stream_index];

//            printf("%d - %ld\n", packet.stream_index, packet.dts);
//            packet.stream_index = av_stream_get_index(inputFormatContext->streams[packet.stream_index]);
//            packet.stream_index = av_find_best_stream(inputFormatContext, packet.stream_index, AVMEDIA_TYPE_UNKNOWN, -1, nullptr, 0);

            ret = av_interleaved_write_frame(outputFormatContext, &packet);
            if (ret < 0)
                throw AvException("Error writing frame", ret);

            av_packet_unref(&packet);
        }

        av_write_trailer(outputFormatContext);

        avformat_close_input(&inputFormatContext);
        avformat_free_context(outputFormatContext);
    }
};


#endif //GOPROSURFSPLITTER_VIDEO_H
