
//=============================================================================
#include "PluginProcessor.h"
#include "PluginEditor.h"
//=============================================================================

//=============================================================================
MidSideV7::MidSideV7()
    :_m_isMidMode(true)
    ,_m_isDoubleLevel(false)
{
}
//=============================================================================
MidSideV7::~MidSideV7()
{
}
//=============================================================================
const String MidSideV7::getName() const
{
    return JucePlugin_Name;
}
//=============================================================================
bool MidSideV7::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}
//=============================================================================
bool MidSideV7::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}
//=============================================================================
bool MidSideV7::silenceInProducesSilenceOut() const
{
    return false;
}
//=============================================================================
double MidSideV7::getTailLengthSeconds() const
{
    return 0.0;
}
//=============================================================================
int MidSideV7::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}
//=============================================================================
int MidSideV7::getCurrentProgram()
{
    return 0;
}
//=============================================================================
void MidSideV7::setCurrentProgram (int index)
{
}
//=============================================================================
const String MidSideV7::getProgramName (int index)
{
    return String();
}
//=============================================================================
void MidSideV7::changeProgramName (int index, const String& newName)
{
}
//=============================================================================
void MidSideV7::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}
//=============================================================================
void MidSideV7::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}
//=============================================================================
void MidSideV7::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    //=========================================================================
    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    //=========================================================================
    if ( getNumInputChannels() == 2 )
    {
        if ( isMidMode() )
            _processMid ( buffer );
        else
            _processSide( buffer );
    }
    //=========================================================================
}
//=============================================================================
bool MidSideV7::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}
//=============================================================================
AudioProcessorEditor* MidSideV7::createEditor()
{
    return new MidSideV7Editor (*this);
}
//=============================================================================
void MidSideV7::getStateInformation (MemoryBlock& destData)
{
    //=========================================================================
    XmlElement xml( "kawaMidSide_Parameter" );
    //=========================================================================
    xml.setAttribute( "isMidMode", isMidMode() );
    xml.setAttribute( "isDoubleLevelMode", isDoubleLevelMode() );
    //=========================================================================
    copyXmlToBinary (xml, destData);
    //=========================================================================
}
//=============================================================================
void MidSideV7::setStateInformation (const void* data, int sizeInBytes)
{
    ScopedPointer<XmlElement> xmlState( getXmlFromBinary (data, sizeInBytes) );
    //=========================================================================
    if ( xmlState != nullptr )
    {
        if ( xmlState->hasTagName( "kawaMidSide_Parameter" ) )
        {
            bool isMideMode_ = xmlState->getBoolAttribute( "isMidMode", false );
            bool isDoubleMode_ = xmlState->getBoolAttribute( "isDoubleLevelMode", false );
            //=================================================================
            setDoubleLevelMode( isDoubleMode_ );
            setMidMode( isMideMode_ );
            //=================================================================
        }
    }
    //=========================================================================
}
//=============================================================================
void MidSideV7::_processMid( AudioSampleBuffer& buffer )
{
    const float* inL = buffer.getReadPointer( 0 );//L 
    const float* inR = buffer.getReadPointer( 1 );//R
    float* outL = buffer.getWritePointer( 0 );
    float* outR = buffer.getWritePointer( 1 );
    int blockSize = buffer.getNumSamples();
    //=========================================================================
    float offsetLevel = 1.0f;
    //=========================================================================
    if ( isDoubleLevelMode() )
        offsetLevel = 2.0f;
    //=========================================================================
    for ( int i = 0; i < blockSize; i++ )
    {
        float mid = (inL[i] +inR[i] )/2;
        outL[i] = mid*offsetLevel;
        outR[i] = mid*offsetLevel;
    }
    //=========================================================================
}
//=============================================================================
void MidSideV7::_processSide( AudioSampleBuffer& buffer )
{
    const float* inL = buffer.getReadPointer( 0 );//L 
    const float* inR = buffer.getReadPointer( 1 );//R
    float* outL = buffer.getWritePointer( 0 );
    float* outR = buffer.getWritePointer( 1 );
    int blockSize = buffer.getNumSamples();
    //=========================================================================
    float offsetLevel = 1.0f;
    //=========================================================================
    if ( isDoubleLevelMode() )
        offsetLevel = 2.0f;
    //=========================================================================
    for ( int i = 0; i < blockSize; i++ )
    {
        float mid = (inL[i] + inR[i]) / 2.0f;
        float sideL = inL[i] - mid;
        float sideR = inR[i] - mid;
        outL[i] = sideL*offsetLevel;
        outR[i] = sideR*offsetLevel;
    }
    //=========================================================================
}
//=============================================================================
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MidSideV7();
}
//=============================================================================
