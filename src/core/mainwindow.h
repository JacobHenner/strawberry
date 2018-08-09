/*
 * Strawberry Music Player
 * This file was part of Clementine.
 * Copyright 2010, David Sansome <me@davidsansome.com>
 *
 * Strawberry is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Strawberry is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Strawberry.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "config.h"

#include <memory>
#include <stdbool.h>

#include <QtGlobal>
#include <QObject>
#include <QWidget>
#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QSortFilterProxyModel>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QPersistentModelIndex>
#include <QMenu>
#include <QAction>
#include <QPoint>
#include <QMimeData>
#include <QList>
#include <QString>
#include <QUrl>
#include <QPixmap>
#include <QTimer>
#include <QSettings>
#include <QtEvents>

#include "core/lazy.h"
#include "core/tagreaderclient.h"
#include "engine/engine_fwd.h"
#include "mac_startup.h"
#include "widgets/osd.h"
#include "collection/collectionmodel.h"
#include "playlist/playlistitem.h"
#include "settings/settingsdialog.h"

class About;
class AlbumCoverManager;;
class Application;
class CollectionViewContainer;
class CommandlineOptions;
class DeviceView;
class DeviceViewContainer;
class EditTagDialog;
class Equalizer;
class ErrorDialog;
class FileView;
class GlobalShortcuts;
class MimeData;
class OrganiseDialog;
class PlaylistListContainer;
class QueueManager;
class Song;
class StatusView;
class SystemTrayIcon;
#if defined(HAVE_GSTREAMER) && defined(HAVE_CHROMAPRINT)
class TagFetcher;
#endif
class TrackSelectionDialog;
#ifdef HAVE_GSTREAMER
class TranscodeDialog;
#endif
class Ui_MainWindow;
class Windows7ThumbBar;
class TidalSearchView;

class MainWindow : public QMainWindow, public PlatformInterface {
  Q_OBJECT

 public:
  MainWindow(Application *app, SystemTrayIcon *tray_icon, OSD *osd, const CommandlineOptions& options, QWidget *parent = nullptr);
  ~MainWindow();

  static const char *kSettingsGroup;
  static const char *kAllFilesFilterSpec;

  // Don't change the values
  enum StartupBehaviour {
    Startup_Remember = 1,
    Startup_AlwaysShow = 2,
    Startup_AlwaysHide = 3,
  };

  // Don't change the values
  enum AddBehaviour {
    AddBehaviour_Append = 1,
    AddBehaviour_Enqueue = 2,
    AddBehaviour_Load = 3,
    AddBehaviour_OpenInNew = 4
  };

  // Don't change the values
  enum PlayBehaviour {
    PlayBehaviour_Never = 1,
    PlayBehaviour_IfStopped = 2,
    PlayBehaviour_Always = 3,
  };

  // Don't change the values
  enum PlaylistAddBehaviour {
    PlaylistAddBehaviour_Play = 1,
    PlaylistAddBehaviour_Enqueue = 2,
  };

  void SetHiddenInTray(bool hidden);
  void CommandlineOptionsReceived(const CommandlineOptions& options);

 protected:
  void keyPressEvent(QKeyEvent *event);
  void resizeEvent(QResizeEvent *event);
  void closeEvent(QCloseEvent *event);

#ifdef Q_OS_WIN32
  bool winEvent(MSG *message, long *result);
#endif

  // PlatformInterface
  void Activate();
  bool LoadUrl(const QString& url);

signals:
  // Signals that stop playing after track was toggled.
  void StopAfterToggled(bool stop);

  void IntroPointReached();

 private slots:
  void FilePathChanged(const QString& path);

  void MediaStopped();
  void MediaPaused();
  void MediaPlaying();
  void TrackSkipped(PlaylistItemPtr item);
  void ForceShowOSD(const Song& song, const bool toggle);

  void PlaylistRightClick(const QPoint& global_pos, const QModelIndex& index);
  void PlaylistCurrentChanged(const QModelIndex& current);
  void PlaylistViewSelectionModelChanged();
  void PlaylistPlay();
  void PlaylistStopAfter();
  void PlaylistQueue();
  void PlaylistSkip();
  void PlaylistRemoveCurrent();
  void PlaylistEditFinished(const QModelIndex& index);
  void EditTracks();
  void EditTagDialogAccepted();
  void RenumberTracks();
  void SelectionSetValue();
  void EditValue();
#if defined(HAVE_GSTREAMER) && defined(HAVE_CHROMAPRINT)
  void AutoCompleteTags();
  void AutoCompleteTagsAccepted();
#endif
  void PlaylistUndoRedoChanged(QAction *undo, QAction *redo);
#ifdef HAVE_GSTREAMER
  void AddFilesToTranscoder();
#endif

#ifdef HAVE_GSTREAMER
  void PlaylistCopyToCollection();
  void PlaylistMoveToCollection();
  void PlaylistCopyToDevice();
  void PlaylistOrganiseSelected(bool copy);
#endif
  //void PlaylistDelete();
  void PlaylistOpenInBrowser();
  void ShowInCollection();

  void ChangeCollectionQueryMode(QAction *action);

  void PlayIndex(const QModelIndex& index);
  void PlaylistDoubleClick(const QModelIndex& index);
  void StopAfterCurrent();

  void SongChanged(const Song& song);
  void VolumeChanged(int volume);

#ifdef HAVE_GSTREAMER
  void CopyFilesToCollection(const QList<QUrl>& urls);
  void MoveFilesToCollection(const QList<QUrl>& urls);
  void CopyFilesToDevice(const QList<QUrl>& urls);
#endif
  void EditFileTags(const QList<QUrl>& urls);

  void AddToPlaylist(QMimeData *data);
  void AddToPlaylist(QAction *action);

  void VolumeWheelEvent(int delta);
  void ToggleShowHide();

  void Seeked(qlonglong microseconds);
  void UpdateTrackPosition();
  void UpdateTrackSliderPosition();

  void TaskCountChanged(int count);

  void ShowCollectionConfig();
  void ReloadSettings();
  void ReloadAllSettings();
  void RefreshStyleSheet();
  void SetHiddenInTray() { SetHiddenInTray(true); }

  void AddFile();
  void AddFolder();
  void AddCDTracks();

  void CommandlineOptionsReceived(const QString& string_options);

  void CheckForUpdates();

  void PlayingWidgetPositionChanged();

  void SongSaveComplete(TagReaderReply *reply, const QPersistentModelIndex& index);

  void ShowCoverManager();

  void ShowAboutDialog();
#ifdef HAVE_GSTREAMER
  void ShowTranscodeDialog();
#endif
  void ShowErrorDialog(const QString& message);
  void ShowQueueManager();
  void EnsureSettingsDialogCreated();
  void EnsureEditTagDialogCreated();
  SettingsDialog *CreateSettingsDialog();
  EditTagDialog *CreateEditTagDialog();
  void OpenSettingsDialog();
  void OpenSettingsDialogAtPage(SettingsDialog::Page page);

  void TabSwitched();
  void SaveGeometry();
  void SavePlaybackStatus();
  void LoadPlaybackStatus();
  void ResumePlayback();

  void Raise();

  void Exit();

  void HandleNotificationPreview(OSD::Behaviour type, QString line1, QString line2);
  void FocusCollectionTab();

  void ShowConsole();

  void FocusTidalSearchField();
  void DoTidalSearch(const QString& query);
  void SearchForArtist();
  void SearchForAlbum();

 private:
  void ConnectStatusView(StatusView *statusview);

  void ApplyAddBehaviour(AddBehaviour b, MimeData *data) const;
  void ApplyPlayBehaviour(PlayBehaviour b, MimeData *data) const;

  void CheckFullRescanRevisions();

  // creates the icon by painting the full one depending on the current position
  QPixmap CreateOverlayedIcon(int position, int scrobble_point);

 private:
  Ui_MainWindow *ui_;
  Windows7ThumbBar *thumbbar_;

  Application *app_;
  SystemTrayIcon  *tray_icon_;
  OSD *osd_;
  Lazy<EditTagDialog> edit_tag_dialog_;
  Lazy<About> about_dialog_;

  GlobalShortcuts *global_shortcuts_;

  CollectionViewContainer *collection_view_;
  StatusView *status_view_;
  FileView *file_view_;
  PlaylistListContainer *playlist_list_;
  DeviceViewContainer *device_view_container_;
  DeviceView *device_view_;

  Lazy<SettingsDialog> settings_dialog_;
  Lazy<AlbumCoverManager> cover_manager_;
  std::unique_ptr<Equalizer> equalizer_;
#ifdef HAVE_GSTREAMER
  Lazy<TranscodeDialog> transcode_dialog_;
#endif
  Lazy<ErrorDialog> error_dialog_;
#ifdef HAVE_GSTREAMER
  Lazy<OrganiseDialog> organise_dialog_;
#endif
  Lazy<QueueManager> queue_manager_;

#if defined(HAVE_GSTREAMER) && defined(HAVE_CHROMAPRINT)
  std::unique_ptr<TagFetcher> tag_fetcher_;
#endif
  std::unique_ptr<TrackSelectionDialog> track_selection_dialog_;
#if defined(HAVE_GSTREAMER) && defined(HAVE_CHROMAPRINT)
  PlaylistItemList autocomplete_tag_items_;
#endif

  TidalSearchView *tidal_search_view_;

  QAction *collection_show_all_;
  QAction *collection_show_duplicates_;
  QAction *collection_show_untagged_;

  QMenu *playlist_menu_;
  QAction *playlist_play_pause_;
  QAction *playlist_stop_after_;
  QAction *playlist_undoredo_;
  //QAction *playlist_organise_;
  QAction *playlist_show_in_collection_;
#ifdef HAVE_GSTREAMER
  QAction *playlist_copy_to_collection_;
  QAction *playlist_move_to_collection_;
  QAction *playlist_copy_to_device_;
  //QAction *playlist_delete_;
#endif
  QAction *playlist_open_in_browser_;
  QAction *playlist_queue_;
  QAction *playlist_skip_;
  QAction *playlist_add_to_another_;
  QList<QAction*> playlistitem_actions_;
  QAction *playlistitem_actions_separator_;
  QAction *search_for_artist_;
  QAction *search_for_album_;

  QModelIndex playlist_menu_index_;

  QSortFilterProxyModel *collection_sort_model_;

  QTimer *track_position_timer_;
  QTimer *track_slider_timer_;
  QSettings settings_;

  bool was_maximized_;
  int saved_playback_position_;
  Engine::State saved_playback_state_;
  AddBehaviour doubleclick_addmode_;
  PlayBehaviour doubleclick_playmode_;
  PlaylistAddBehaviour doubleclick_playlist_addmode_;
  PlayBehaviour menu_playmode_;

};

#endif  // MAINWINDOW_H

