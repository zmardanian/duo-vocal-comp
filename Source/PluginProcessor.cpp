#include "PluginProcessor.h"
#include "PluginEditor.h"

using APVTS = juce::AudioProcessorValueTreeState;

namespace
{
juce::AudioParameterFloatAttributes fmt (std::function<juce::String (float)> toText,
                                         std::function<float (const juce::String&)> fromText = nullptr)
{
    auto a = juce::AudioParameterFloatAttributes()
                 .withStringFromValueFunction ([toText] (float v, int) { return toText (v); });
    if (fromText == nullptr)
        fromText = [] (const juce::String& t) { return t.getFloatValue(); };
    return a.withValueFromStringFunction (fromText);
}

juce::String dbText (float v)  { return juce::String (v, 1) + " dB"; }
juce::String pctText (float v) { return juce::String (juce::roundToInt (v)) + " %"; }

const juce::Identifier kPresetNameId ("presetName");
} // namespace

APVTS::ParameterLayout DuoCompProcessor::createLayout()
{
    using namespace juce;
    std::vector<std::unique_ptr<RangedAudioParameter>> p;

    //== v1 parameters: IDs, ranges and ORDER are frozen (saved projects / automation) ==
    // FET (1176-style)
    p.push_back (std::make_unique<AudioParameterBool> (ParameterID { "fetOn", 1 }, "FET On", true));
    p.push_back (std::make_unique<AudioParameterFloat> (ParameterID { "fetIn", 1 }, "FET Input",
        NormalisableRange<float> (-12.0f, 30.0f, 0.1f), 6.0f, fmt (dbText)));
    p.push_back (std::make_unique<AudioParameterFloat> (ParameterID { "fetOut", 1 }, "FET Output",
        NormalisableRange<float> (-24.0f, 24.0f, 0.1f), 0.0f, fmt (dbText)));

    NormalisableRange<float> atk (0.02f, 0.8f, 0.001f);
    atk.setSkewForCentre (0.2f);
    p.push_back (std::make_unique<AudioParameterFloat> (ParameterID { "fetAtk", 1 }, "FET Attack", atk, 0.2f,
        fmt ([] (float v) { return String (roundToInt (v * 1000.0f)) + String::fromUTF8 (" \xc2\xb5s"); },
             [] (const String& t) { return t.getFloatValue() * 0.001f; })));

    NormalisableRange<float> rel (50.0f, 1100.0f, 1.0f);
    rel.setSkewForCentre (250.0f);
    p.push_back (std::make_unique<AudioParameterFloat> (ParameterID { "fetRel", 1 }, "FET Release", rel, 150.0f,
        fmt ([] (float v) { return String (roundToInt (v)) + " ms"; })));

    p.push_back (std::make_unique<AudioParameterChoice> (ParameterID { "fetRatio", 1 }, "FET Ratio",
        StringArray { "4:1", "8:1", "12:1", "20:1", "All" }, 0));

    // Opto (LA-2A-style)
    p.push_back (std::make_unique<AudioParameterBool> (ParameterID { "optoOn", 1 }, "Opto On", true));
    p.push_back (std::make_unique<AudioParameterFloat> (ParameterID { "optoPR", 1 }, "Opto Peak Reduction",
        NormalisableRange<float> (0.0f, 100.0f, 0.1f), 30.0f,
        fmt ([] (float v) { return String (v, 1); })));
    p.push_back (std::make_unique<AudioParameterFloat> (ParameterID { "optoGain", 1 }, "Opto Gain",
        NormalisableRange<float> (-12.0f, 24.0f, 0.1f), 0.0f, fmt (dbText)));
    p.push_back (std::make_unique<AudioParameterChoice> (ParameterID { "optoMode", 1 }, "Opto Mode",
        StringArray { "Compress", "Limit" }, 0));

    // Global
    p.push_back (std::make_unique<AudioParameterChoice> (ParameterID { "order", 1 }, "Order",
        StringArray { "FET > Opto", "Opto > FET" }, 0));

    NormalisableRange<float> hpf (20.0f, 300.0f, 1.0f);
    hpf.setSkewForCentre (80.0f);
    p.push_back (std::make_unique<AudioParameterFloat> (ParameterID { "scHpf", 1 }, "Sidechain HPF", hpf, 80.0f,
        fmt ([] (float v) { return String (roundToInt (v)) + " Hz"; })));

    p.push_back (std::make_unique<AudioParameterFloat> (ParameterID { "color", 1 }, "Color",
        NormalisableRange<float> (0.0f, 100.0f, 1.0f), 30.0f, fmt (pctText)));
    p.push_back (std::make_unique<AudioParameterFloat> (ParameterID { "mix", 1 }, "Mix",
        NormalisableRange<float> (0.0f, 100.0f, 1.0f), 100.0f, fmt (pctText)));
    p.push_back (std::make_unique<AudioParameterFloat> (ParameterID { "out", 1 }, "Output",
        NormalisableRange<float> (-24.0f, 24.0f, 0.1f), 0.0f, fmt (dbText)));

    //== v2 parameters (appended so existing parameter indices never move) ====
    p.push_back (std::make_unique<AudioParameterFloat> (ParameterID { "link", 2 }, "Stereo Link",
        NormalisableRange<float> (0.0f, 100.0f, 1.0f), 100.0f, fmt (pctText)));
    p.push_back (std::make_unique<AudioParameterFloat> (ParameterID { "optoEmph", 2 }, "Opto HF Emphasis",
        NormalisableRange<float> (0.0f, 100.0f, 1.0f), 0.0f, fmt (pctText)));
    p.push_back (std::make_unique<AudioParameterChoice> (ParameterID { "os", 2 }, "Oversampling",
        StringArray { "1x", "2x", "4x" }, 1));
    p.push_back (std::make_unique<AudioParameterBool> (ParameterID { "scListen", 2 }, "SC Listen", false));
    p.push_back (std::make_unique<AudioParameterBool> (ParameterID { "autoGain", 2 }, "Auto Makeup", false));

    return { p.begin(), p.end() };
}

DuoCompProcessor::DuoCompProcessor()
    : AudioProcessor (BusesProperties()
                          .withInput ("Input", juce::AudioChannelSet::stereo(), true)
                          .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      apvts (*this, nullptr, "PARAMS", createLayout())
{
    pFetOn    = apvts.getRawParameterValue ("fetOn");
    pFetIn    = apvts.getRawParameterValue ("fetIn");
    pFetOut   = apvts.getRawParameterValue ("fetOut");
    pFetAtk   = apvts.getRawParameterValue ("fetAtk");
    pFetRel   = apvts.getRawParameterValue ("fetRel");
    pFetRatio = apvts.getRawParameterValue ("fetRatio");
    pOptoOn   = apvts.getRawParameterValue ("optoOn");
    pOptoPR   = apvts.getRawParameterValue ("optoPR");
    pOptoGain = apvts.getRawParameterValue ("optoGain");
    pOptoMode = apvts.getRawParameterValue ("optoMode");
    pOrder    = apvts.getRawParameterValue ("order");
    pScHpf    = apvts.getRawParameterValue ("scHpf");
    pColor    = apvts.getRawParameterValue ("color");
    pMix      = apvts.getRawParameterValue ("mix");
    pOut      = apvts.getRawParameterValue ("out");
    pLink     = apvts.getRawParameterValue ("link");
    pEmph     = apvts.getRawParameterValue ("optoEmph");
    pOs       = apvts.getRawParameterValue ("os");
    pListen   = apvts.getRawParameterValue ("scListen");
    pAuto     = apvts.getRawParameterValue ("autoGain");

    // the oversampling factor changes on the audio thread (after a 3 ms fade); the new
    // latency is reported to the host from the message thread
    startTimerHz (20);
}

DuoCompProcessor::~DuoCompProcessor()
{
    stopTimer();
}

bool DuoCompProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    const auto out = layouts.getMainOutputChannelSet();
    if (out != juce::AudioChannelSet::mono() && out != juce::AudioChannelSet::stereo())
        return false;
    return out == layouts.getMainInputChannelSet();
}

duo::Params DuoCompProcessor::readParams() const
{
    duo::Params p;
    p.fetOn      = pFetOn->load() > 0.5f;
    p.fetInDb    = pFetIn->load();
    p.fetOutDb   = pFetOut->load();
    p.fetAtkMs   = pFetAtk->load();
    p.fetRelMs   = pFetRel->load();
    p.fetRatio   = juce::roundToInt (pFetRatio->load());
    p.optoOn     = pOptoOn->load() > 0.5f;
    p.optoPR     = pOptoPR->load();
    p.optoGainDb = pOptoGain->load();
    p.optoMode   = juce::roundToInt (pOptoMode->load());
    p.order      = juce::roundToInt (pOrder->load());
    p.scHpfHz    = pScHpf->load();
    p.colorPct   = pColor->load();
    p.mixPct     = pMix->load();
    p.outDb      = pOut->load();
    p.linkPct    = pLink->load();
    p.emphasisPct = pEmph->load();
    p.osLog2     = juce::roundToInt (pOs->load());
    p.scListen   = pListen->load() > 0.5f;
    p.autoMakeup = pAuto->load() > 0.5f;
    return p;
}

void DuoCompProcessor::syncEngineParams (bool bypass)
{
    auto p = readParams();
    p.bypass = bypass;
    dsp.setParams (p);
}

void DuoCompProcessor::prepareToPlay (double sampleRate, int)
{
    preparedChannels = juce::jlimit (1, duo::kMaxCh, getTotalNumOutputChannels());
    dsp.prepare (sampleRate, preparedChannels);
    syncEngineParams (false);
    dsp.reset();
    setLatencySamples (dsp.getLatencySamples());
}

void DuoCompProcessor::reset()
{
    if (preparedChannels > 0)
        dsp.reset();
}

juce::String DuoCompProcessor::getPresetName() const
{
    const juce::ScopedLock sl (nameLock);
    return presetName;
}

void DuoCompProcessor::setPresetName (const juce::String& n)
{
    const juce::ScopedLock sl (nameLock);
    presetName = n;
}

bool DuoCompProcessor::isPresetModified() const
{
    const auto a = readParams();
    const auto& b = presetReference;
    auto diff = [] (float x, float y) { return std::abs (x - y) > 1.0e-3f; };
    return a.fetOn != b.fetOn || diff (a.fetInDb, b.fetInDb) || diff (a.fetOutDb, b.fetOutDb)
        || diff (a.fetAtkMs, b.fetAtkMs) || diff (a.fetRelMs, b.fetRelMs) || a.fetRatio != b.fetRatio
        || a.optoOn != b.optoOn || diff (a.optoPR, b.optoPR) || diff (a.optoGainDb, b.optoGainDb)
        || a.optoMode != b.optoMode || a.order != b.order || diff (a.scHpfHz, b.scHpfHz)
        || diff (a.colorPct, b.colorPct) || diff (a.mixPct, b.mixPct) || diff (a.outDb, b.outDb)
        || diff (a.linkPct, b.linkPct) || diff (a.emphasisPct, b.emphasisPct) || a.osLog2 != b.osLog2
        || a.scListen != b.scListen || a.autoMakeup != b.autoMakeup;
}

void DuoCompProcessor::markPresetReference()
{
    presetReference = readParams();
}

void DuoCompProcessor::timerCallback()
{
    if (programNamePending.exchange (false))
    {
        setPresetName (getProgramName (currentProgram.load()));
        presetReference = duo::factoryPresets()[(size_t) currentProgram.load()].params;
    }
    if (preparedChannels > 0 && dsp.getLatencySamples() != getLatencySamples())
        setLatencySamples (dsp.getLatencySamples());
}

void DuoCompProcessor::storeMax (std::atomic<float>& a, float v) noexcept
{
    float cur = a.load (std::memory_order_relaxed);
    while (v > cur && ! a.compare_exchange_weak (cur, v, std::memory_order_relaxed))
    {
    }
}

void DuoCompProcessor::publishMeters()
{
    const auto& m = dsp.getMeters();
    for (int c = 0; c < 2; ++c)
    {
        storeMax (meters.inPeak[c], m.inPeak[c]);
        storeMax (meters.outPeak[c], m.outPeak[c]);
    }
    storeMax (meters.fetGr, m.fetGrDb);
    storeMax (meters.optoGr, m.optoGrDb);
    meters.makeup.store (m.makeupDb, std::memory_order_relaxed);
    dsp.clearMeters();
}

void DuoCompProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    const int numIn = getTotalNumInputChannels();
    const int numOut = getTotalNumOutputChannels();
    const int n = buffer.getNumSamples();
    for (int c = numIn; c < numOut; ++c)
        buffer.clear (c, 0, n);
    if (preparedChannels == 0 || n == 0)
        return;

    syncEngineParams (false);
    dsp.process (buffer.getArrayOfWritePointers(), juce::jmin (buffer.getNumChannels(), preparedChannels), n);
    publishMeters();
}

// host bypass: keep running the engine so the output stays latency-aligned and the switch
// is a 10 ms crossfade instead of a click
void DuoCompProcessor::processBlockBypassed (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    const int n = buffer.getNumSamples();
    for (int c = getTotalNumInputChannels(); c < getTotalNumOutputChannels(); ++c)
        buffer.clear (c, 0, n);
    if (preparedChannels == 0 || n == 0)
        return;

    syncEngineParams (true);
    dsp.process (buffer.getArrayOfWritePointers(), juce::jmin (buffer.getNumChannels(), preparedChannels), n);
    publishMeters();
}

//==============================================================================
void DuoCompProcessor::applyParams (const duo::Params& p)
{
    auto set = [this] (const char* id, float value)
    {
        if (auto* param = apvts.getParameter (id))
            param->setValueNotifyingHost (param->convertTo0to1 (value));
    };
    set ("fetOn", p.fetOn ? 1.0f : 0.0f);
    set ("fetIn", p.fetInDb);
    set ("fetOut", p.fetOutDb);
    set ("fetAtk", p.fetAtkMs);
    set ("fetRel", p.fetRelMs);
    set ("fetRatio", (float) p.fetRatio);
    set ("optoOn", p.optoOn ? 1.0f : 0.0f);
    set ("optoPR", p.optoPR);
    set ("optoGain", p.optoGainDb);
    set ("optoMode", (float) p.optoMode);
    set ("order", (float) p.order);
    set ("scHpf", p.scHpfHz);
    set ("color", p.colorPct);
    set ("mix", p.mixPct);
    set ("out", p.outDb);
    set ("link", p.linkPct);
    set ("optoEmph", p.emphasisPct);
    set ("os", (float) p.osLog2);
    set ("scListen", p.scListen ? 1.0f : 0.0f);
    set ("autoGain", p.autoMakeup ? 1.0f : 0.0f);
}

int DuoCompProcessor::getNumPrograms()
{
    return (int) duo::factoryPresets().size();
}

void DuoCompProcessor::setCurrentProgram (int index)
{
    const auto list = duo::factoryPresets();
    if (index < 0 || index >= (int) list.size())
        return;
    currentProgram.store (index);
    applyParams (list[(size_t) index].params);
    if (juce::MessageManager::existsAndIsCurrentThread())
    {
        setPresetName (list[(size_t) index].name);
        presetReference = list[(size_t) index].params;
    }
    else
        programNamePending.store (true); // no String allocation on the audio thread
}

const juce::String DuoCompProcessor::getProgramName (int index)
{
    const auto list = duo::factoryPresets();
    if (index < 0 || index >= (int) list.size())
        return {};
    return list[(size_t) index].name;
}

juce::File DuoCompProcessor::userPresetFolder()
{
    auto base = juce::SystemStats::getEnvironmentVariable ("XDG_DATA_HOME", {});
    juce::File root = base.isNotEmpty() ? juce::File (base)
                                        : juce::File::getSpecialLocation (juce::File::userHomeDirectory)
                                              .getChildFile (".local/share");
    return root.getChildFile ("DuoVocalComp").getChildFile ("Presets");
}

bool DuoCompProcessor::saveUserPreset (const juce::String& name)
{
    const auto clean = juce::File::createLegalFileName (name.trim());
    if (clean.isEmpty())
        return false;
    auto folder = userPresetFolder();
    if (! folder.createDirectory())
        return false;
    auto state = apvts.copyState();
    state.setProperty (kPresetNameId, name.trim(), nullptr);
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    if (xml == nullptr || ! xml->writeTo (folder.getChildFile (clean + ".duopreset")))
        return false;
    setPresetName (name.trim());
    markPresetReference();
    return true;
}

bool DuoCompProcessor::loadUserPreset (const juce::File& file)
{
    auto xml = juce::XmlDocument::parse (file);
    if (xml == nullptr || ! xml->hasTagName (apvts.state.getType()))
        return false;
    auto tree = juce::ValueTree::fromXml (*xml);
    restoreTree (tree);
    setPresetName (tree.getProperty (kPresetNameId, file.getFileNameWithoutExtension()).toString());
    markPresetReference();
    return true;
}

void DuoCompProcessor::selectSlot (int slot)
{
    slot = juce::jlimit (0, 1, slot);
    if (slot == activeSlot)
        return;
    slots[activeSlot] = apvts.copyState();
    if (slots[slot].isValid())
        restoreTree (slots[slot].createCopy());
    else
        slots[slot] = apvts.copyState();
    activeSlot = slot;
}

void DuoCompProcessor::copyActiveToOther()
{
    slots[1 - activeSlot] = apvts.copyState();
}

// APVTS::replaceState skips a parameter whose *snapped* value already matches the tree, so a
// raw value written by a host (e.g. 0.64 on a bool) could survive a restore (found by pluginval's
// state-restoration test). Re-apply every stored value explicitly after replacing the state.
void DuoCompProcessor::restoreTree (const juce::ValueTree& tree)
{
    apvts.replaceState (tree);
    for (auto* p : getParameters())
        if (auto* rp = dynamic_cast<juce::RangedAudioParameter*> (p))
        {
            const auto child = apvts.state.getChildWithProperty ("id", rp->paramID);
            if (child.isValid() && child.hasProperty ("value"))
            {
                const float norm = rp->convertTo0to1 ((float) child.getProperty ("value"));
                if (std::abs (rp->getValue() - norm) > 1.0e-6f)
                    rp->setValueNotifyingHost (norm);
            }
        }
}

//==============================================================================
void DuoCompProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    state.setProperty (kPresetNameId, getPresetName(), nullptr);
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void DuoCompProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml (getXmlFromBinary (data, sizeInBytes));
    if (xml != nullptr && xml->hasTagName (apvts.state.getType()))
    {
        auto tree = juce::ValueTree::fromXml (*xml);
        setPresetName (tree.getProperty (kPresetNameId, getPresetName()).toString());
        restoreTree (tree);
        markPresetReference(); // a restored session counts as 'unmodified' until you change something
    }
}

juce::AudioProcessorEditor* DuoCompProcessor::createEditor()
{
    return new DuoCompEditor (*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DuoCompProcessor();
}
