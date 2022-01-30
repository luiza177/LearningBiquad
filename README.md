# Learning Biquad

Learning Biquad is an audio plug-in (VST3, AU, and standalone for testing purposes) made with the [JUCE framework](https://juce.com/) for the purpose of visualizing how [digital biquad filters](https://en.wikipedia.org/wiki/Digital_biquad_filter) work. Uses CMake instead of Projucer.

The formulas used to calculate each filter's coefficients are from [Will Pirkle](https://www.willpirkle.com/)'s excellent book *Designing Audio Effect Plugins in C++*, and the DSP was implemented from scratch (ie. the JUCE DSP module was **not** used).

## Roadmap
* Implent HPF and BPF.
* Toggle buttons for filter type, moving frequency Slider changes coefficient sliders.
* Allow for filter order.
### Maybe
* Response curve
* Biquad graphic




