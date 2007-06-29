#ifndef RoadSearchCircleSeed_h
#define RoadSearchCircleSeed_h

//
// Package:         RecoTracker/RoadSearchSeedFinder
// Class:           RoadSearchCircleSeed
// 
// Description:     circle from three global points in 2D 
//                  all data members restricted to 2 dimensions
//
//                  following http://home.att.net/~srschmitt/circle3pts.html
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Mon Jan 22 21:42:35 UTC 2007
//
// $Author: gutsche $
// $Date: 2007/06/13 14:26:47 $
// $Revision: 1.3 $
//

#include <utility>
#include <vector>
#include <iosfwd>

#include "DataFormats/GeometryVector/interface/GlobalPoint.h"

#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"

#include "RecoTracker/RoadMapRecord/interface/Roads.h"

class RoadSearchCircleSeed
{
 public:

  // line, first parameter slope, second offset
  typedef std::pair<double,double> line;

  enum type {
    circle,
    straightLine
  };
  
  RoadSearchCircleSeed(const TrackingRecHit *hit1,
		       const TrackingRecHit *hit2,
		       const TrackingRecHit *hit3,
		       GlobalPoint point1,
		       GlobalPoint point2,
		       GlobalPoint point3);
  RoadSearchCircleSeed(const TrackingRecHit *hit1,
		       const TrackingRecHit *hit2,
		       GlobalPoint point1,
		       GlobalPoint point2);

  ~RoadSearchCircleSeed();

  inline std::vector<GlobalPoint>  Points() const { return points_; }
  inline std::vector<GlobalPoint>::const_iterator begin_points() const { return points_.begin(); }
  inline std::vector<GlobalPoint>::const_iterator end_points()   const { return points_.end();   }

  inline std::vector<const TrackingRecHit*> Hits() const { return hits_; }
  inline std::vector<const TrackingRecHit*>::const_iterator begin_hits() const { return hits_.begin(); }
  inline std::vector<const TrackingRecHit*>::const_iterator end_hits()   const { return hits_.end();   }

  inline GlobalPoint  Center()          const { return center_;}
  inline double       Radius()          const { return radius_;}
  inline double       ImpactParameter() const { return impactParameter_;}
  inline double       Type()            const { return type_; }

  inline void                   setSeed(const Roads::RoadSeed *input) { seed_ = input; }
  inline const Roads::RoadSeed* getSeed()                             { return seed_;  }

  inline void                   setSet(const Roads::RoadSet *input)   { set_ = input; }
  inline const Roads::RoadSet*  getSet()                              { return set_;  }

  bool Compare(const RoadSearchCircleSeed *circle,
	       double centerCut,
	       double radiusCut,
	       unsigned int differentHitsCut) const;

  bool CompareCenter(const RoadSearchCircleSeed *circle,
		     double centerCut) const;

  bool CompareRadius(const RoadSearchCircleSeed *circle,
		     double radiusCut) const;

  bool CompareDifferentHits(const RoadSearchCircleSeed *circle,
			    unsigned int differentHitsCut) const;

  double determinant(double array[][3], unsigned int bins);
  double calculateImpactParameter(GlobalPoint center,
				  double radius);

  std::string print() const;

 private:

  std::vector<GlobalPoint> points_;

  std::vector<const TrackingRecHit*> hits_;

  type             type_;
  GlobalPoint      center_;
  double           radius_;
  double           impactParameter_;

  const Roads::RoadSeed *seed_;
  const Roads::RoadSet  *set_;

};

std::ostream& operator<<(std::ostream& ost, const RoadSearchCircleSeed & seed);

#endif
