#ifndef VENUS_EFFECT_H_
#define VENUS_EFFECT_H_

#include <opencv2/core.hpp>

namespace venus {

enum class RangeMode
{
	SHADOWS,
	MIDTONES,
	HIGHLIGHTS,
};
class Effect
{
private:
	static float mapColorBalance(float value, float lightness, float shadows, float midtones, float highlights);

public:
	/**
	 * Tone is a color term commonly used by painters. Toning a bitmap with specified color, it's 
	 * like mixing the color pixel by pixel, the color resulting in layering a color 
	 * <code>dst</code> RGB with a <code>src</code> color RGB is:
	 * 
	 * <pre>
	 * new_R = dst_R + (src_R - dst_R) * amount
	 * new_G = dst_G + (src_G - dst_G) * amount
	 * new_B = dst_B + (src_B - dst_B) * amount
	 * </pre>
	 * 
	 * @see {@link https://en.wikipedia.org/wiki/Tints_and_shades}
	 * 
	 * @param[out] dst
	 * @param[in]  src
	 * @param[in]  color  RGBA memory layout
	 * @param[in]  amount Color blending amount, with 0 being unchanged, with 1 being the pure color.
	 * @return
	 */
	static void tone(cv::Mat& dst, const cv::Mat& src, uint32_t color, float amount);

	// "inline static" vs "static inline" https://lwn.net/Articles/142278/
	static inline void shade(cv::Mat& dst, const cv::Mat& src, float amount)   { tone(dst, src, 0x00000000/* black */, amount); }
	static inline void tint (cv::Mat& dst, const cv::Mat& src, float amount)   { tone(dst, src, 0x00FFFFFF/* white */, amount); }
	static inline void tone (cv::Mat& dst, const cv::Mat& src, uint32_t color) { tone(dst, src, color, (color>>24)/255.0F);     }

	/**
	 * Note that <code>dst</code> can be same as <code>src</code> image.
	 *
	 * @param[out] dst    The output image.
	 * @param[in]  src    The input image.
	 * @param[in]  radius Blur radius.
	 */
	static void gaussianBlur(cv::Mat& dst, const cv::Mat& src, float radius);
	inline static void gaussianBlur(cv::Mat& image, float radius) { gaussianBlur(image, image, radius); }

	/**
	 * color balance is the global adjustment of the intensities of the colors (typically red, green, and blue primary colors).
	 * @see https://en.wikipedia.org/wiki/Color_balance for details.
	 *
	 * @param[out] dst
	 * @param[in]  src
	 * @param[in]  config Array of {shadows, midtones, highlights}, each RangeMode is a Vec3f(CYAN_RED, MAGENTA_GREEN, YELLOW_BLUE)
	 *                    each channel is in range [-1.0, 1.0].
	 * @param[in] preserve_luminosity
	 */
	static void adjustColorBalance(float* const dst, const float* const src, int width, int height, const cv::Vec3f config[3], bool preserve_luminosity);
//	static void adjustColorBalance(uint8_t* const dst, const uint8_t* const src, int width, int height, const cv::Vec3f config[3], bool preserve_luminosity);
//	static void adjustColorBalance(cv::Mat& dst, const cv::Mat& src, const cv::Vec3f config[3], bool preserve_luminosity);
	
};

} /* namespace venus */
#endif /* VENUS_EFFECT_H_ */