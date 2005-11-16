/*
 * Copyright (c) 2005 The University of Notre Dame. All Rights Reserved.
 *
 * The University of Notre Dame grants you ("Licensee") a
 * non-exclusive, royalty free, license to use, modify and
 * redistribute this software in source and binary code form, provided
 * that the following conditions are met:
 *
 * 1. Acknowledgement of the program authors must be made in any
 *    publication of scientific results based in part on use of the
 *    program.  An acceptable form of acknowledgement is citation of
 *    the article in which the program was described (Matthew
 *    A. Meineke, Charles F. Vardeman II, Teng Lin, Christopher
 *    J. Fennell and J. Daniel Gezelter, "OOPSE: An Object-Oriented
 *    Parallel Simulation Engine for Molecular Dynamics,"
 *    J. Comput. Chem. 26, pp. 252-271 (2005))
 *
 * 2. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 3. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 * This software is provided "AS IS," without a warranty of any
 * kind. All express or implied conditions, representations and
 * warranties, including any implied warranty of merchantability,
 * fitness for a particular purpose or non-infringement, are hereby
 * excluded.  The University of Notre Dame and its licensors shall not
 * be liable for any damages suffered by licensee as a result of
 * using, modifying or distributing the software or its
 * derivatives. In no event will the University of Notre Dame or its
 * licensors be liable for any lost revenue, profit or data, or for
 * direct, indirect, special, consequential, incidental or punitive
 * damages, however caused and regardless of the theory of liability,
 * arising out of the use of or inability to use software, even if the
 * University of Notre Dame has been advised of the possibility of
 * such damages.
 */
#include <algorithm> 
#include <stack>
#include "io/SectionParserManager.hpp"
#include "utils/Trim.hpp"
#include "utils/simError.h"

namespace oopse {

  SectionParserManager::~SectionParserManager() {
    SectionParserManager::iterator i;
    for (i = sectionParsers_.begin(); i != sectionParsers_.end(); ++i) {
      delete (i->sectionParser);
    }
    sectionParsers_.clear();
  }

  void SectionParserManager::parse(std::istream& input, ForceField& ff) {

    //reset active flags
    SectionParserManager::iterator i;
    for (i = sectionParsers_.begin(); i != sectionParsers_.end(); ++i) {
      i->isActive = false;
    }

    const int bufferSize = 65535;
    char buffer[bufferSize];
    int lineNo = 0;
    std::stack<std::string> sectionNameStack;
    //scan through the input stream and find section names        
    while(input.getline(buffer, bufferSize)) {
      ++lineNo;
        
      std::string line = trimLeftCopy(buffer);
      //a line begins with "//" is a comment line
      if ( line.empty() || (line.size() >= 2 && line[0] == '/' 
                            && line[1] == '/') ) {
	continue;
      } else {        
	StringTokenizer tokenizer(line);
	if (tokenizer.countTokens() < 2) {
	  continue;
	} else {
	  std::string keyword = tokenizer.nextToken();

	  if (keyword == "begin") {
	    std::string section = tokenizer.nextToken();
	    sectionNameStack.push(section);
 
	    i = std::find_if(sectionParsers_.begin(), sectionParsers_.end(),
                       SameSectionParserFunctor(section));
	    if (i == sectionParsers_.end()){
	      sprintf(painCave.errMsg, 
                "SectionParserManager Error: Can not find corresponding "
                "section parser for %s\n",
                section.c_str());
	      painCave.isFatal = 1;
	      simError();                        
	    } else {
	      if (i->isActive) {
          sprintf(painCave.errMsg, "SectionParserManager Error:find multiple %s "
                  "section\n",
                  section.c_str());
          painCave.isFatal = 1;
          simError();                        
	      } else {                         
          i->isActive = true;
          i->lineNo = lineNo;
          i->offset = input.tellg();
	      }
	    }
	  } else if (keyword == "end") {
	    std::string section = tokenizer.nextToken();
	    if (sectionNameStack.top() == section) {
	      sectionNameStack.pop();
	    } else {
	      sprintf(painCave.errMsg, "SectionParserManager Error: begin %s and end %s does not match at line %d\n",
                sectionNameStack.top().c_str(), section.c_str(), lineNo);
	      painCave.isFatal = 1;
	      simError();
	    }
      
	  } else {
	    continue;
	  }
	}
      }
      
    }
    
    if (!sectionNameStack.empty()) {
      sprintf(painCave.errMsg, "SectionParserManager Error: stack is not empty\n");
      painCave.isFatal = 1;
      simError();
    }
    
    //invoke parser
    for (i = sectionParsers_.begin(); i != sectionParsers_.end(); ++i) {
      if (i->isActive) {
        //C++ standard does not guarantee seekg  reset EOF, in that case, seekg will fail
        //It is always a good idea to call clear() before seek
        input.clear();            
        input.seekg(i->offset);
        (i->sectionParser)->parse(input, ff, i->lineNo);
        (i->sectionParser)->validateSection();
      }
    }
    
  }
  
  void SectionParserManager::push_front(SectionParser* sp) {
    SectionParserManager::iterator i;
    i = findSectionParser(sp->getSectionName());
    if (i != sectionParsers_.end()) {
      std::cerr << sp->getSectionName() << " section parser is alway existed" 
      << std::endl;
      return;
    }
    
    SectionParserContext context;
    
    if (sectionParsers_.empty()) {
      context.priority = beginPriority_;
    } else {
      context.priority = sectionParsers_.front().priority - priorityDifference_;
    }
    
    context.sectionParser = sp;
    context.lineNo = 0;
    context.offset = 0;
    context.isActive = false;

    sectionParsers_.push_front(context);
  }

  void SectionParserManager::push_back(SectionParser* sp) {
    SectionParserManager::iterator i;
    i = findSectionParser(sp->getSectionName());
    if (i != sectionParsers_.end()) {
      std::cerr << sp->getSectionName() << " section parser is alway existed" 
      << std::endl;
      return;
    }

    SectionParserContext context;    
    if (sectionParsers_.empty()) {
      context.priority = beginPriority_;
    } else {
      context.priority = sectionParsers_.back().priority + priorityDifference_;
    }

    context.sectionParser = sp;
    context.lineNo = 0;
    context.offset = 0;
    context.isActive = false;

    sectionParsers_.push_back(context);

  }

  void SectionParserManager::insert(SectionParser* sp, int priority) {
    SectionParserManager::iterator i;
    i = findSectionParser(sp->getSectionName());
    if (i != sectionParsers_.end()) {
      std::cerr << sp->getSectionName() << " section parser is already existed" 
      << std::endl;
    }

    SectionParserContext context;    
    context.priority = priority;
    context.sectionParser = sp;
    context.lineNo = 0;
    context.offset = 0;
    context.isActive = false;

    if (sectionParsers_.empty()) {
      sectionParsers_.push_back(context);
    } else {

      for (i = sectionParsers_.begin(); i != sectionParsers_.end(); ++i) {
	if (i->priority == priority) {
	  std::cerr << "Priority " << priority << " already used" << std::endl;
	  return;
	} else if (i->priority > priority) {
	  sectionParsers_.insert(i, context);
	  break;
	}
            
      }
    }

  }


  SectionParserManager::iterator SectionParserManager::findSectionParser(const std::string& sectionName) {
    SectionParserManager::iterator i;
    for (i = sectionParsers_.begin(); i != sectionParsers_.end(); ++i) {
      if (i->sectionParser->getSectionName() == sectionName) {
        break;
      }
    }

    return i;
  }

}
