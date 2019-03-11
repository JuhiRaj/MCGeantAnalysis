/**
 *  @copyright Copyright 2017 The J-PET Framework Authors. All rights reserved.
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may find a copy of the License in the LICENCE file.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *  @file EventCategorizer.cpp
 */

#include <iostream> 
#include <JPetWriter/JPetWriter.h> 
#include "EventCategorizer.h"
#include "../j-pet-framework/GeantParser/JPetGeantEventInformation/JPetGeantEventInformation.h"
#include "../j-pet-framework/GeantParser/JPetGeantDecayTree/JPetGeantDecayTree.h"
#include "../j-pet-framework/GeantParser/JPetGeantEventPack/JPetGeantEventPack.h"
#include "../j-pet-framework/GeantParser/JPetGeantParser/JPetGeantParser.h"
#include "../j-pet-framework/GeantParser/JPetGeantParser/JPetGeantParserTools.h"
#include "../j-pet-framework/GeantParser/JPetGeantScinHits/JPetGeantScinHits.h"
#include "../j-pet-framework/GeantParser/JPetRandom/JPetRandom.h"
#include "../j-pet-framework/GeantParser/JPetSmearingFunctions/JPetSmearingFunctions.h"


using namespace std;

EventCategorizer::EventCategorizer(const char * name): JPetUserTask(name) {}

bool EventCategorizer::init() {

  INFO("Event categorization started.");
  INFO("Looking at two hit Events on Layer 1&2 only - creating only control histograms");

  fOutputEvents = new JPetTimeWindow("JPetEvent");

  if (fSaveControlHistos) {



    getStatistics().createHistogram(new TH1F("Hits_All", "Event Multiplicity",
      11, -0.5, 10.5));
    getStatistics().getHisto1D("Hits_All")->GetXaxis()->SetTitle("Event Multiplicity");
    getStatistics().getHisto1D("Hits_All")->GetYaxis()->SetTitle("Counts");

    getStatistics().createHistogram(new TH1F("Hits", "Event Multiplicity - 4 Hits",
      11, -0.5, 10.5));
    getStatistics().getHisto1D("Hits")->GetXaxis()->SetTitle("Event Multiplicity - 4 Hits");
    getStatistics().getHisto1D("Hits")->GetYaxis()->SetTitle("Counts");




 }


}


bool EventCategorizer::exec() {

  //Begin of exec function

  if (auto timeWindow = dynamic_cast <
    const JPetTimeWindow *
      const > (fEvent)) {
    uint n = timeWindow->getNumberOfEvents();
    for (uint i = 0; i < n; ++i) {



      const auto & event = dynamic_cast <
        const JPetEvent & > (timeWindow->operator[](i));



      getStatistics().getHisto1D("Hits_All")->Fill(event.getHits().size());


      if (event.getHits().size() >= 4) {


      getStatistics().getHisto1D("Hits")->Fill(event.getHits().size()); 


	vector<JPetMCHit> hits = event.getHits();	


	for (auto i = hits.begin(); i != hits.end(); ++i) 
	{
		

		auto& hit1 = *i;


		int multiplicity = hit1.getGenGammaMultiplicity();

		cout<<multiplicity<<endl;



						}



					}


				}

	
			}

else {
  return false;
}
return true;
}

bool EventCategorizer::terminate() {

  INFO("More than one hit Events done. Writing conrtrol histograms.");
  return true;
}

void EventCategorizer::saveEvents(const vector < JPetEvent > & events) {
  for (const auto & event: events) {
    fOutputEvents->add < JPetEvent > (event);
  }
}
