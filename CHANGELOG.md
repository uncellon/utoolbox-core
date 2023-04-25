# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## Unreleased

## [0.0.10] - 2023-04-25
### Changed
- Small refactoring.

## [0.0.9] - 2022-09-29
### Changed
- Operator() in delegate now is const.

## [0.0.8] - 2022-09-20
### Fixed
- Deadlock in EventDispatcher when getInstance() and lockAttachments called at
the same time v2.

## [0.0.7] - 2022-06-06
### Added
- Default event loop.

## [0.0.6] - 2022-06-03
### Fixed
- Bug with allocating multiple event dispatchers.

## [0.0.5] - 2022-06-02
### Fixed
- Removed extra mutex to avoid condition variable miss in event loop.

## [0.0.4] - 2022-06-02
### Fixed
- Deadlock in EventDispatcher when getInstance() and lockAttachments called at 
the same time.

## [0.0.3] - 2022-05-16
### Changed
- Code style.

## [0.0.2] - 2022-04-21
### Added
- Object(Object* parent = nullptr) to make it easier to attach to the event 
loop.

## [0.0.1] - 2022-04-19
### Added
- Sources :)

[0.0.10]: https://github.com/uncellon/utoolbox-core/releases/tag/v0.0.10
[0.0.9]: https://github.com/uncellon/utoolbox-core/releases/tag/v0.0.9
[0.0.8]: https://github.com/uncellon/utoolbox-core/releases/tag/v0.0.8
[0.0.7]: https://github.com/uncellon/utoolbox-core/releases/tag/v0.0.7
[0.0.6]: https://github.com/uncellon/utoolbox-core/releases/tag/v0.0.6
[0.0.5]: https://github.com/uncellon/utoolbox-core/releases/tag/v0.0.5
[0.0.4]: https://github.com/uncellon/utoolbox-core/releases/tag/v0.0.4
[0.0.3]: https://github.com/uncellon/utoolbox-core/releases/tag/v0.0.3
[0.0.2]: https://github.com/uncellon/utoolbox-core/releases/tag/v0.0.2
[0.0.1]: https://github.com/uncellon/utoolbox-core/releases/tag/v0.0.1