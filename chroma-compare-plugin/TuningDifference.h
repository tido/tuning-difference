#ifndef TUNING_DIFFERENCE_H
#define TUNING_DIFFERENCE_H

#include <vamp-sdk/Plugin.h>

#include <cq/CQSpectrogram.h>

#include <memory>

using std::string;
using std::vector;

class TuningDifference : public Vamp::Plugin
{
public:
    TuningDifference(float inputSampleRate);
    virtual ~TuningDifference();

    string getIdentifier() const;
    string getName() const;
    string getDescription() const;
    string getMaker() const;
    int getPluginVersion() const;
    string getCopyright() const;

    InputDomain getInputDomain() const;
    size_t getPreferredBlockSize() const;
    size_t getPreferredStepSize() const;
    size_t getMinChannelCount() const;
    size_t getMaxChannelCount() const;

    ParameterList getParameterDescriptors() const;
    float getParameter(string identifier) const;
    void setParameter(string identifier, float value);

    ProgramList getPrograms() const;
    string getCurrentProgram() const;
    void selectProgram(string name);

    OutputList getOutputDescriptors() const;

    bool initialise(size_t channels, size_t stepSize, size_t blockSize);
    void reset();

    FeatureSet process(const float *const *inputBuffers,
                       Vamp::RealTime timestamp);

    FeatureSet getRemainingFeatures();

protected:
    typedef vector<float> Signal;
    typedef vector<double> Chroma;
    typedef vector<double> TFeature;

    int m_bpo;
    std::unique_ptr<CQSpectrogram> m_refCQ;
    Chroma m_refTotals;
    TFeature m_refFeature;
    Signal m_other;
    int m_blockSize;
    int m_frameCount;

    CQParameters paramsForTuningFrequency(double hz) const;
    TFeature computeFeatureFromTotals(const Chroma &totals) const;
    TFeature computeFeatureFromSignal(const Signal &signal, double hz) const;
    double featureDistance(const TFeature &other, int rotation = 0) const;
    int findBestRotation(const TFeature &other) const;

    mutable std::map<string, int> m_outputs;
};


#endif