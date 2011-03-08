/*********************************************************************
 * Author: Patrick Sébastien
 * http://www.workinprogress.ca
 *********************************************************************/
 
#ifndef __main__
#define __main__

#define VERSION "4683"

// main wxWidgets header file
#include <wx/wx.h>
#include <wx/thread.h>
#include <wx/html/htmlwin.h>
#include <wx/accel.h>
#include <wx/taskbar.h>
#include <wx/notifmsg.h>
#include <wx/process.h>
#include <wx/socket.h>
#include <wx/stdpaths.h>
#include <wx/uri.h>
#include <wx/filesys.h>
#include <wx/fs_inet.h>

// gzip - tar
#include <iostream>
#include <memory>
#include <wx/zipstrm.h>
#include <wx/zstream.h>
#include <wx/tarstrm.h>

// json
#include "jsonval.h"
#include "jsonreader.h"
#include "jsonwriter.h"

// gui
#include "gui.h"

// v2c
#include "v2capplication.h"
#include "v2ceditor.h"

// active word
#include "activeword.h"

// julius
#include "julius.h"

// web thread
#include "web.h"
// download buffer
static const size_t DLBUFSIZE = 4096;

// language downloader
//#include "languagedownloader.h"
#include "language.h"

//monitor process
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h> // for opendir(), readdir(), closedir()
#include <sys/stat.h> // for stat()
#include <cstdlib>
#include <cstring>
#include <cstdarg>

#define PROC_DIRECTORY "/proc/"
#define CASE_SENSITIVE    1
#define CASE_INSENSITIVE  0
#define EXACT_MATCH       1
#define INEXACT_MATCH     0

#define PROCESSTIMER_ID 10000
#define PRETRIGTIMER_ID 10001
#define UNKNOWNTIMER_ID 10002

#define POLLING 2000 // monitor process
#define PRETRIGTIME 10000 // how long to hold for the action word

// hash
WX_DECLARE_HASH_MAP( int, int, wxIntegerHash, wxIntegerEqual, pidrahash );
WX_DECLARE_HASH_MAP( int, int, wxIntegerHash, wxIntegerEqual, pidcounthash );

////////////////////////////////////////////////////////////////////////////////
// application class declaration 
////////////////////////////////////////////////////////////////////////////////
class MainApp : public wxApp
{
	public:
		virtual bool OnInit();
};

// declare global static function wxGetApp()
DECLARE_APP(MainApp)

////////////////////////////////////////////////////////////////////////////////
// main application frame declaration 
////////////////////////////////////////////////////////////////////////////////
class JuliusThread; // forward declaration
class MainTaskBarIcon; // forward declaration

class MainFrame : public MainFrameBase
{
	public:
		MainFrame( wxWindow *parent );
		virtual ~MainFrame();
		
		// julius
		Julius *m_Julius;
		JuliusThread *m_pThread;
        wxCriticalSection m_pThreadCS;
		void startjulius();
		void pauser(bool state);
		void onJuliusPronun(wxCommandEvent& event);
        void onJuliusSentence(wxCommandEvent& event);
        void onJuliusReady(wxCommandEvent& event);
        void onJuliusScore(wxCommandEvent& event);
        void onJuliusWatch(wxCommandEvent& event);
        void onJuliusLevelMeter(wxCommandEvent& event);
		
		// web
		Web *m_pWebThread;
        wxCriticalSection m_pWebCS;
		void OnWeb(wxCommandEvent& event);
		
		// language downloader thread
		//LanguageDownloader *m_pLanguageDownloaderThread;
        //wxCriticalSection m_pLanguageDownloaderCS;
		//void OnLDUP(wxCommandEvent& event);
		//void OnLDEND(wxCommandEvent& event);
		
		// language
		void OnLanguageProgess(int, int);
		
		// active word
		virtual void Onb_activeword( wxCommandEvent& event );
		void Dob_activeword();
		
		// v2c
		void v2cloading();
		void V2cApplicationReload();
		void V2cShortcutReload();
		void Dopb_v2capplication();
		void Dopb_v2cshortcut();
		void autocomplete();
		
		virtual void OnCloseFrame( wxCloseEvent& event );
		void OnQuit();
		
protected:

		// notbook
		void Onm_nb( wxNotebookEvent& event );
		void Onm_nbv2c( wxNotebookEvent& event );
		
		// active word
		ActiveWord *m_activeword;
		
		// julius
		void startjuliusthread();
		void juliusgentlyexit();
		void readjuliusconf();
		void writejuliusconf(wxString opt);
		void Onb_restartjulius( wxCommandEvent& event );
		
		// engine
		void Onb_update( wxCommandEvent& event );
		void Onc_engdriver( wxCommandEvent& event );
		void Ons_englevel(wxScrollEvent& event);
		void Oncb_engdefault(wxCommandEvent& event);
		void Onsp_engthreshold(wxSpinEvent& event);
		void Onsp_engzero(wxSpinEvent& event);
		void Oncb_engnostrip(wxCommandEvent& event);
		void Oncb_engzmean(wxCommandEvent& event);
		void Onc_enggprune(wxCommandEvent& event);
		void Onc_engiwcd1(wxCommandEvent& event);
		void Onsp_engiwcd1(wxSpinEvent& event);
		void Onsp_engtmix(wxSpinEvent& event);
		void Onsp_engbeam(wxSpinEvent& event);
		void Ontc_engpenalty(wxCommandEvent& event);
		
		// web
		void startwebthread(wxString what);
		void webexit();
		//void startlanguagedownloaderthread();
		//void languagedownloaderexit();
		
		// language
		Language *m_pLanguage;
		void Onb_languagedownload( wxCommandEvent& event );
		bool languagedownload();
		wxString language;
		
		// dictionary
		void writedictionary();
		
		// v2c
		virtual void Onrb_v2cmethod( wxCommandEvent& event );
		virtual void Oncb_v2cmonitor( wxCommandEvent& event );
		virtual void Oncb_v2clauncher( wxCommandEvent& event );
		
		// application
		V2cApplication* dv2capp;
		V2cEditor* dv2ceditor;
		void Onpb_v2capplication( wxCommandEvent& event );
		void Onb_v2capplicationedit( wxCommandEvent& event );
		void Onpc_v2capplication( wxUpdateUIEvent& event );
		void V2cApplicationLoad();
		
		// shortcut
		void Onpb_v2cshortcut( wxCommandEvent& event );
		void Onpc_v2cshortcut( wxUpdateUIEvent& event );
		void Onpb_v2cshortcutedit( wxCommandEvent& event );
		void V2cShortcutLoad();
		
		// import
		void Onb_v2cimportapp( wxCommandEvent& event );
		void Onb_v2cimportshortcut( wxCommandEvent& event );
		void Onpc_v2cimportapp( wxCommandEvent& event );
		void Onpc_v2cimportshortcut( wxCommandEvent& event );
		void importhtml(wxString v2c);
		bool listv2c(wxString v2c);
		bool getv2c(wxString v2c);
		bool downloadv2c(wxString server, wxString tgz);
		bool gunzipv2c(wxString v2c);
		void importsuccess();
		//void Onb_v2cimportfp( wxCommandEvent& event );
		//void Onb_v2cimportpf( wxUpdateUIEvent& event );
		
		wxArrayString appurl;
		wxArrayString appurlkey;
		wxArrayString appurlhtml;
		wxArrayString shortcuturl;
		wxArrayString shortcuturlkey;
		wxArrayString shortcuturlhtml;
		
		// preference
		void readpreference();
		void writepreference();
		void createpreference();
		void Onm_prefupdate( wxCommandEvent& event );

		// pause
		void Oncb_pause( wxCommandEvent& event );
		void autopause();
		wxStopWatch ap_timer;
        wxStopWatch aup_timer;
		bool aup_userpause;
		bool aup_timer_started;
		bool aup_timer_ispaused;
		
		// work
		void searchandexecute(wxString word);
		void Eye(wxString txt);
		long Hand(wxString type, wxString cmd);
		
		// various
		void Onc_notification( wxCommandEvent& event );
		void OnLink( wxHtmlLinkEvent& event );

		// taskbar
		MainTaskBarIcon *m_taskBarIcon;

		// timer for process
		wxTimer *m_timer;
		void OnMonitorTimer(wxTimerEvent& event);
		int IsNumeric(const char* ccharptr_CharacterList);
		
		// timer for pretrig
		wxTimer *pretrigm_timer;
		void OnPreTrigTimer(wxTimerEvent& event);
		wxStopWatch pretrig_timer;
		
		// timer for unknown word
		wxTimer *unknownm_timer;
		void OnUnknownTimer(wxTimerEvent& event);
		wxStopWatch unknown_timer;
		bool unknown;

		// regex
		wxRegEx onlyint;
		int regexonlyint(wxString conftxt);
		
		// load v2c
        void v2cloading(wxString file, long pid);
		void v2cloadingprocessname();
		
		// process
		wxProcess* proc;
        long m_pidLast;
		
		// standard path
		wxStandardPaths stdpath;
		wxString GetCurrentWorkingDirectory();
		
		bool actionwaiting;
		double score;
		double threshold;
		bool webupdateicon;
		wxString updateurl;
		
		int nbmistake;
		int triggering;
		int actionning;
		int duration;

		DECLARE_EVENT_TABLE()
};

const int SENTENCE_ID = 100001;
const int PRONUN_ID = 100002;
const int READY_ID = 100003;
const int SCORE_ID = 100004;
const int WATCH_ID = 100005;
const int LEVELMETER_ID = 100006;
const int WEB_ID = 100007;
const int LDUP_ID = 100008;
const int LDEND_ID = 100009;



// thread
class JuliusThread : public wxThread
{
    public:
        JuliusThread(MainFrame *handler)
            : wxThread(wxTHREAD_DETACHED)
            { m_pHandler = handler; }
        ~JuliusThread();

    protected:
        virtual ExitCode Entry();
        MainFrame *m_pHandler;
};



// taskbar
class MainTaskBarIcon: public wxTaskBarIcon
{
	public:
		MainTaskBarIcon(MainFrame *handler) {
			m_pHandler = handler;
			check = false;
		}
        void OnLeftButtonDClick(wxTaskBarIconEvent&);
        void OnMenuExit(wxCommandEvent&);
		void OnMenuWebsite(wxCommandEvent& );
		void OnMenuApp(wxCommandEvent&);
		void OnMenuShortcut(wxCommandEvent&);
		void OnMenuActiveWord(wxCommandEvent&);
		void OnShowKiku(wxCommandEvent&);
		void OnMenuUICheckmark(wxUpdateUIEvent&);
        virtual wxMenu *CreatePopupMenu();
		
	protected:
		MainFrame *m_pHandler;
		bool check;
		
	DECLARE_EVENT_TABLE()
};


// This is the handler for process termination events
class Process : public wxProcess
{
public:
    Process(MainFrame *parent, const wxString& cmd)
        : wxProcess(parent), m_cmd(cmd)
    {
        m_parent = parent;
    }
    virtual void OnTerminate(int pid, int status);

protected:
    MainFrame *m_parent;
    wxString m_cmd;
};

#endif //__main__