// You can edit anything you want in this file.
// However you need to implement all public Plains function, as provided below as a template

#include "plains25a2.h"


Plains::Plains()
{

}

Plains::~Plains()
{
    
}

StatusType Plains::add_team(int teamId)
{
  if(teamId <= 0) return StatusType::INVALID_INPUT;
  if(herds.search(teamId)) return StatusType::FAILURE;
  try {
      teams.makeSet(Herd(teamId));
  } catch(bad_alloc &e) {
    return StatusType::ALLOCATION_ERROR;
  }
  return StatusType::SUCCESS;
}

StatusType Plains::add_jockey(int jockeyId, int teamId)
{
  if(jockeyId <= 0 || teamId <= 0) return StatusType::INVALID_INPUT;

  auto team = herds.search(teamId);

  if(team->getData()->Deleted() || riders.search(jockeyId)) return StatusType::FAILURE;
  shared_ptr<Rider> rider;
  try{
    rider = make_shared<Rider>(jockeyId, team);
  } catch (bad_alloc &e) {
    return StatusType::ALLOCATION_ERROR;
  }

  if (!riders.insert(rider)) throw std::logic_error("riders.insert() failed");

  return StatusType::SUCCESS;
}

StatusType Plains::update_match(int victoriousJockeyId, int losingJockeyId)
{
  if (losingJockeyId <= 0 || victoriousJockeyId <= 0
      || losingJockeyId == victoriousJockeyId) return StatusType::INVALID_INPUT;

    auto winRider = riders.search(victoriousJockeyId),
        loseRider = riders.search(losingJockeyId);
    if (!winRider || !loseRider) return StatusType::FAILURE;

  int winHerdId = winRider->HerdID(),
      loseHerdId = loseRider->HerdID();

  auto winningTeam = teams.Find(Herd(winHerdId)),
       loseTeam = teams.Find(Herd(loseHerdId));

  if (winningTeam->getID() == loseTeam->getID()) return StatusType::FAILURE;

  winningTeam->increaseRecord(1);
  loseTeam->increaseRecord(-1);

  return StatusType::SUCCESS;
}

StatusType Plains::merge_teams(int teamId1, int teamId2)
{

    return StatusType::FAILURE;
}

StatusType Plains::unite_by_record(int record)
{
    return StatusType::FAILURE;
}

output_t<int> Plains::get_jockey_record(int jockeyId)
{
    return 0;
}

output_t<int> Plains::get_team_record(int teamId)
{
    return 0;
}
