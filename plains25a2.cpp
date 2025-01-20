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
      auto team = teams.makeSet(Herd(teamId));
      herds.insert(team->getHead());
      shared_ptr<RecordsNode<Set<Herd>>> record = records.insert(0, team);
      team->setRecord(record);
  } catch(bad_alloc &e) {
    return StatusType::ALLOCATION_ERROR;
  }
  return StatusType::SUCCESS;
}

StatusType Plains::add_jockey(int jockeyId, int teamId)
{
  if(jockeyId <= 0 || teamId <= 0) return StatusType::INVALID_INPUT;

  auto team = herds.search(teamId);

  if(!team || team->getData()->Deleted() || riders.search(jockeyId)) return StatusType::FAILURE;
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

  winRider->won();
  loseRider->lost();
  shared_ptr<RecordsNode<Set<Herd>>> loseRecord = loseTeam->getNodeRecord();
  shared_ptr<RecordsNode<Set<Herd>>> record = 
    records.insert(loseRecord->getRecord() - 1, loseTeam);
  loseTeam->setRecord(record);
  records.deleteNode(loseRecord);
  shared_ptr<RecordsNode<Set<Herd>>> winRecord = winningTeam->getNodeRecord();
  record = records.insert(winRecord->getRecord() + 1, winningTeam);
  winningTeam->setRecord(record);
  records.deleteNode(winRecord);

  return StatusType::SUCCESS;
}

StatusType Plains::merge_teams(int teamId1, int teamId2)
{
  // input check

  if (teamId1 <= 0 || teamId2 <= 0 ||
        teamId1 == teamId2) return StatusType::INVALID_INPUT;
  auto team1 = herds.search(teamId1),
        team2 = herds.search(teamId2);
  // check if teams still alive
  if (!team1 || !team2 ||
      team1->getData()->Deleted() ||
      team2->getData()->Deleted())
    return StatusType::FAILURE;

  // make union
  try {
    teams.Union(Herd(teamId1),Herd(teamId2), this->records);
    // shared_ptr<Set<Herd>> find1 = teams.Find(Herd(teamId1));
    // shared_ptr<Set<Herd>> find2 = teams.Find(Herd(teamId2));
    // shared_ptr<RecordsNode<Set<Herd>>> firstRecord = find1->getNodeRecord();
    // shared_ptr<RecordsNode<Set<Herd>>> secondRecord = find2->getNodeRecord();
    // shared_ptr<Set<Herd>> lead ;
    // if (firstRecord->getRecord() > secondRecord->getRecord())
    // { lead = find1; }else{ lead = find2;}
    // shared_ptr<RecordsNode<Set<Herd>>> record = 
    // records.insert(firstRecord->getRecord() + secondRecord->getRecord() , lead);
    // records.deleteNode(firstRecord);
    // records.deleteNode(secondRecord);
    // lead->setRecord(record);

  } catch (std::invalid_argument &e) {
   return StatusType::FAILURE;
  }

  return StatusType::SUCCESS;
}

StatusType Plains::unite_by_record(int record)
{
  if(record <= 0){
    return StatusType::INVALID_INPUT;
  }
  Pairs groups = records.Amount(record);
  if(groups.first == nullptr || groups.second == nullptr){
    return StatusType::FAILURE;
  }
  try{
    merge_teams(groups.first->key(), groups.second->key());
  }catch(...){
    return StatusType::ALLOCATION_ERROR;
  }
  return StatusType::SUCCESS;
}

output_t<int> Plains::get_jockey_record(int jockeyId)
{
  if (jockeyId <= 0) return StatusType::INVALID_INPUT;
  auto jockey = riders.search(jockeyId);
  if (!jockey) return StatusType::FAILURE;

  return jockey->getRecord();
}

output_t<int> Plains::get_team_record(int teamId)
{
    if (teamId <= 0) return StatusType::INVALID_INPUT;
    auto team = herds.search(teamId);
    if (!team || team->getData()->Deleted()) return StatusType::FAILURE;
    if (!team->getSet()){
      throw logic_error("team->getSet() is null");
      }
    return team->getSet()->getRecord();
}
